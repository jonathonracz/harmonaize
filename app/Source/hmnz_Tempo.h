/*
  ==============================================================================

    hmnz_Tempo.h
    Created: 1 Feb 2018 12:37:20am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"
#include "hmnz_Automation.h"
#include "hmnz_TempoMap.h"

class Tempo : public ValueTreeObject<IDs::Tempo>
{
public:
    Tempo (const ValueTree& v, UndoManager* um)
        : ValueTreeObject (v, um), automation (Automation<double>::createDefaultState(), um, 120.0),
          tempoMap (v.getRoot(), automation)
    {
        getState().addChild (automation.getState(), -1, nullptr);
    }

    /** Returns the beat for a given time in seconds. */
    double beat (double time) const noexcept
    {
        return tempoMap.beat (time * 60.0);
    }

    /** Returns the time in seconds for a given beat. */
    double time (double beat) const noexcept
    {
        return tempoMap.time (beat) * 60.0;
    }

    /** Returns the tempo (in BPM) at a given time in seconds. */
    double tempoAtTime (double time) const noexcept
    {
        return tempoMap.tempoAtTime (time * 60.0);
    }

    /** Returns the tempo (in BPM) at a given beat. */
    double tempoAtBeat (double beat) const noexcept
    {
        return tempoMap.tempoAtBeat (beat);
    }

    /** Returns the end time of the edit (total time), in seconds. */
    double endTime() const noexcept
    {
        return tempoMap.endTime() * 60.0;
    }

    Automation<double> automation;

private:
    TempoMap tempoMap;
};
