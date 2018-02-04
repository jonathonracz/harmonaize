/*
  ==============================================================================

    hmnz_TempoMap.h
    Created: 3 Feb 2018 11:11:01am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_TempoMapIntervalArray.h"

class TempoMap  : public ValueTreeObject<IDs::TempoMap>
{
public:
    TempoMap (const ValueTree& v, UndoManager* um, Automation<double>& automation)
        : ValueTreeObject (v, um), automationSource (automation), editRoot (getState().getRoot().getType()), intervals (v, um)
    {
        automationSource.getState().addListener (this);
        jassert (editRoot.getType() == IDs::Edit);
        editRoot.addListener (this);

        editStartMarker = Automation<double>::createDefaultState();
        editEndMarker = Automation<double>::createDefaultState();

        for (int i = 0; i < automationSource.getState().getNumChildren() + 1; ++i)
            getState().addChild (TempoMapInterval::createDefaultState(), -1, nullptr);

        recomputeTempoIntervals();
    }

    double beat (double time) const noexcept
    {
        if (intervals.objects.size() == 0)
        {
            jassert (automationSource.getState().getNumChildren() == 1);
            return time * tempoAtBeat(0.0); // The tempo should be the same everywhere.
        }

        for (TempoMapInterval* interval : intervals.objects)
            if (interval->containsTime (time))
                return interval->time (time);

        jassertfalse;
        return std::numeric_limits<double>::min();
    }

    double time (double beat) const noexcept
    {
        if (intervals.objects.size() == 0)
        {
            jassert (automationSource.getState().getNumChildren() == 1);
            return beat / tempoAtBeat (0.0); // The tempo should be the same everywhere.
        }

        for (TempoMapInterval* interval : intervals.objects)
            if (interval->containsBeat (beat))
                return interval->beat (beat);

        jassertfalse;
        return std::numeric_limits<double>::min();
    }

    double tempoAtTime (double time) const noexcept
    {
        return tempoAtBeat (beat (time));
    }

    double tempoAtBeat (double beat) const noexcept
    {
        return automationSource.getValueAtBeat (beat);
    }

private:
    ValueTree editStartMarker;
    ValueTree editEndMarker;
    Automation<double>& automationSource;
    ValueTree editRoot;
    TempoMapIntervalArray intervals;

    void recomputeTempoIntervals()
    {
        jassert (getState().getNumChildren() == automationSource.getState().getNumChildren() + 1);
        for (int i = 0; i < getState().getNumChildren(); ++i)
        {
            ValueTree beforeMarker;
            if (i == 0)
                beforeMarker = editStartMarker;
            else
                beforeMarker = automationSource.getState().getChild (i - 1);

            ValueTree afterMarker;
            if (i == automationSource.getState().getNumChildren())
                afterMarker = editEndMarker;
            else
                afterMarker = automationSource.getState().getChild (i);

            intervals.objects[i]->setMarkers (beforeMarker, afterMarker);
        }
    }

    void valueTreeChildAdded (ValueTree& parent, ValueTree& addedChild) override
    {
        if (parent == automationSource.getState())
        {
            getState().addChild (ValueTreeObject<IDs::TempoMapInterval>::createDefaultState(), -1, nullptr);
            recomputeTempoIntervals();
        }
    }

    void valueTreeChildRemoved (ValueTree& parent, ValueTree& removedChild, int indexRemovedFrom) override
    {
        if (parent == automationSource.getState())
        {
            getState().removeChild (0, nullptr);
            recomputeTempoIntervals();
        }
    }

    void valueTreePropertyChanged (ValueTree& treeChanged, const Identifier& property) override
    {
        if (treeChanged == editRoot && (property == IDs::EditProps::OriginBeat || property == IDs::EditProps::EndBeat))
            recomputeTempoIntervals();
    }
};
