/*
  ==============================================================================

    hmnz_Tempo.h
    Created: 1 Feb 2018 12:37:20am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"
#include "hmnz_CacheValueWrappers.h"

class Tempo : public ValueTreeObject<IDs::Tempo>
{
public:
    struct Snapshot
    {
        double time;
        double beat;
        double bpm;
    };

    Tempo (const ValueTree& v, UndoManager* um)
        : ValueTreeObject (v, um),
          beatsPerMinute (getState(), IDs::TempoProps::BeatsPerMinute, getUndoManager(), 120.0)
    {
    }

    /** Returns the beat for a given time in seconds. */
    double beat (double time) const
    {
        return beatForTime (time, beatsPerMinute.get());
    }

    /** Returns the time in seconds for a given beat. */
    double time (double beat) const
    {
        return timeForBeat (beat, beatsPerMinute.get());
    }

    /** Returns the tempo (in BPM) at a given time in seconds. */
    double tempoAtTime (double time) const
    {
        return beatsPerMinute.get();
    }

    /** Returns the tempo (in BPM) at a given beat. */
    double tempoAtBeat (double beat) const
    {
        return beatsPerMinute.get();
    }

    void setTempoAtTime (double tempo, double time)
    {
        beatsPerMinute.setValue (tempo, getUndoManager());
    }

    void setTempoAtBeat (double tempo, double beat)
    {
        beatsPerMinute.setValue (tempo, getUndoManager());
    }

    Snapshot snapshotAtTime (double time) const
    {
        double bpm = beatsPerMinute.get();
        Snapshot ret;
        ret.time = time;
        ret.beat = beatForTime (time, bpm);
        ret.bpm = bpm;
        return ret;
    }

private:
    CachedValue<SPSCAtomicWrapper<double>> beatsPerMinute;

    static double beatForTime (double time, double bpm)
    {
        return (bpm / 60.0) * time;
    }

    static double timeForBeat (double beat, double bpm)
    {
        return (60.0 / bpm) * beat;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Tempo)
};
