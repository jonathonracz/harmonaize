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

class ClipList  : public ValueTreeObject<IDs::ClipList>
{
public:
    ClipList (const ValueTree& v, UndoManager* um, Track* const _owner)
        : ValueTreeObject (v, um), owner (_owner), clips (v, um, owner)
    {
        getState().addListener (this);
    }

    Array<Clip*> getClipsForInterval(double start, double end) const noexcept
    {
        Array<Clip*> ret;
        for (Clip* clip : clips.objects)
            if (clip->start >= start && clip->start + clip->length < end)
                ret.add (clip);

        return ret;
    }

private:
    Track* const owner;
    ClipArray clips;

    void valueTreePropertyChanged (ValueTree& treeChanged, const Identifier& property) override
    {
        if (treeChanged.getParent() == getState() && (property == IDs::ClipProps::Start || property == IDs::ClipProps::Length))
        {
            fixClipOverlaps (treeChanged);
            sortClips();
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

    void sortClips() noexcept
    {
        StartComparator comparator;
        getState().sort (comparator, nullptr, false);
    }

    void fixClipOverlaps (const ValueTree& dominantClip) noexcept
    {
        Array<int> childrenToDelete;
        Clip* changedChild = clips.objects[getState().indexOf (dominantClip)];
        for (int i = 0; i < clips.objects.size(); ++i)
        {
            Clip* child = clips.objects[i];
            if (child == changedChild)
                continue;

            if (child->start >= changedChild->start && child->length <= changedChild->length)
                childrenToDelete.add (i);
                else if (changedChild->start < (child->start + child->length))
                    child->length = changedChild->start - child->start;
                    else if (changedChild->start + changedChild->length > child->start)
                        child->start = changedChild->start + changedChild->length;
                        }

        for (int i = childrenToDelete.size() - 1; i >= 0; --i)
            getState().removeChild (i, getUndoManager());
    }
};
