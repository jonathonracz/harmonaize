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

    double beat (double time) const noexcept
    {
        return tempoMap.beat (time);
    }

    double time (double beat) const noexcept
    {
        return tempoMap.time (beat);
    }

    double tempoAtTime (double time) const noexcept
    {
        return tempoMap.tempoAtTime (time);
    }

    double tempoAtBeat (double beat) const noexcept
    {
        return tempoMap.tempoAtBeat (beat);
    }

    double endTime() const noexcept
    {
        return tempoMap.endTime();
    }

    Automation<double> automation;

private:
    TempoMap tempoMap;
};
