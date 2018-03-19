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
                  public HomogeneousValueTreeObjectArray<Clip>::Listener,
                  public ValueTree::Listener
{
public:
    ClipList (const ValueTree& v, UndoManager* um, Track& _track)
        : ValueTreeObject (v, um), clips (v, um, _track)
    {
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
    void objectAdded (Clip* clip, int, HomogeneousValueTreeObjectArray<Clip>*) override
    {
        fixClipOverlaps (clip);
        sortClips();
    }

    void valueTreePropertyChanged (ValueTree& treeChanged, const Identifier& property) override
    {
        if (treeChanged.getParent() == getState())
        {
            if (property == IDs::ClipProps::Start || property == IDs::ClipProps::Length)
            {
                Clip* changedClip = clips[clips.indexOfStateInObjects (treeChanged)];
                fixClipOverlaps (changedClip);
                sortClips();
            }
        }
    }

    void valueTreeChildAdded (ValueTree& parent, ValueTree& child) override {}
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

    void fixClipOverlaps (const Clip* dominantClip)
    {
        Array<int> childrenToDelete;
        for (int i = 0; i < clips.size(); ++i)
        {
            Clip* child = clips[i];
            if (child == dominantClip)
                continue;

            if (child->start >= dominantClip->start && child->length <= dominantClip->length)
                childrenToDelete.add (i);
            else if (dominantClip->start < (child->start + child->length))
                child->length.setValue (dominantClip->start.get() - child->start.get(), getUndoManager());
            else if (dominantClip->start + dominantClip->length > child->start)
                child->start.setValue (dominantClip->start.get() + dominantClip->length.get(), getUndoManager());
        }

        for (int i = childrenToDelete.size() - 1; i >= 0; --i)
            getState().removeChild (i, getUndoManager());
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ClipList)
};
