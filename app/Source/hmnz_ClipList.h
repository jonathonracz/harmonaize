/*
  ==============================================================================

    hmnz_ClipList.h
    Created: 8 Feb 2018 7:54:48pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"
#include "hmnz_ClipArray.h"
#include "hmnz_Clip.h"

class Track;

class ClipList  : public ValueTreeObject<IDs::ClipList>,
                  public ValueTree::Listener
{
public:
    ClipList (const ValueTree& v, UndoManager* um, Track& _track)
        : ValueTreeObject (v, um), clips (v, um, _track)
    {
        getState().addListener (this);
    }

    Array<Clip*> getClipsForInterval (double start, double end) const
    {
        Array<Clip*> ret;
        for (Clip* clip : clips)
            if (clip->start >= start && clip->start + clip->length < end)
                ret.add (clip);

        return ret;
    }

    Clip* clipAtTime (double time) const
    {
        for (Clip* clip : clips)
            if (clip->start >= time && clip->start + clip->length < time)
                return clip;

        return nullptr;
    }

    ClipArray clips;

private:
    void valueTreePropertyChanged (ValueTree& treeChanged, const Identifier& property) override
    {
        jassert (treeChanged != getState());
        if (treeChanged.getParent() == getState())
        {
            if (property == IDs::ClipProps::Start || property == IDs::ClipProps::Length)
            {
                fixClipOverlaps (treeChanged);
                sortClips();
            }
        }
    }

    void valueTreeChildAdded (ValueTree& parent, ValueTree& child) override
    {
        if (parent == getState())
        {
            fixClipOverlaps (child);
            sortClips();
        }
    }

    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

    struct StartComparator
    {
        int compareElements (const ValueTree& first, const ValueTree& second)
        {
            double firstStart = first[IDs::ClipProps::Start];
            double secondStart = second[IDs::ClipProps::Start];
            jassert (first != second);
            return (firstStart < secondStart) ? -1 : 1;
        }
    };

    void sortClips()
    {
        StartComparator comparator;
        getState().sort (comparator, nullptr, false);
    }

    void fixClipOverlaps (const ValueTree& dominantClip)
    {
        Array<int> childrenToDelete;
        Clip* changedChild = clips[getState().indexOf (dominantClip)];
        for (int i = 0; i < clips.size(); ++i)
        {
            Clip* child = clips[i];
            if (child == changedChild)
                continue;

            if (child->start >= changedChild->start && child->length <= changedChild->length)
                childrenToDelete.add (i);
            else if (changedChild->start < (child->start + child->length))
                child->length.setValue (changedChild->start.get() - child->start.get(), getUndoManager());
            else if (changedChild->start + changedChild->length > child->start)
                child->start.setValue (changedChild->start.get() + changedChild->length.get(), getUndoManager());
        }

        for (int i = childrenToDelete.size() - 1; i >= 0; --i)
            getState().removeChild (i, getUndoManager());
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ClipList)
};
