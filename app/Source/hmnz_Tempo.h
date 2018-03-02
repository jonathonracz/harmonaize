/*
  ==============================================================================

    hmnz_Tempo.h
    Created: 1 Feb 2018 12:37:20am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"

class Tempo : public ValueTreeObject<IDs::Tempo>
{
public:
    Tempo (const ValueTree& v, UndoManager* um)
        : ValueTreeObject (v, um),
          beatsPerMinute (getState(), IDs::TempoProps::BeatsPerMinute, getUndoManager(), 120.0)
    {
    }

    /** Returns the beat for a given time in seconds. */
    double beat (double time) const noexcept
    {
        return (beatsPerMinute / 60.0) * time;
    }

    /** Returns the time in seconds for a given beat. */
    double time (double beat) const noexcept
    {
        return (60.0 / beatsPerMinute) * beat;
    }

    /** Returns the tempo (in BPM) at a given time in seconds. */
    double tempoAtTime (double time) const noexcept
    {
        return beatsPerMinute;
    }

    /** Returns the tempo (in BPM) at a given beat. */
    double tempoAtBeat (double beat) const noexcept
    {
        return beatsPerMinute;
    }

    CachedValue<double> beatsPerMinute;
};
