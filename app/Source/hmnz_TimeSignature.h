/*
  ==============================================================================

    hmnz_TimeSignature.h
    Created: 7 Feb 2018 3:21:46pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"
#include "hmnz_Utility.h"

class TimeSignature : public ValueTreeObject<IDs::TimeSignature>
{
public:
    TimeSignature (const ValueTree& v, UndoManager* um)
        : ValueTreeObject (v, um),
          numerator (getState(), IDs::TimeSigNumerator, getUndoManager(), 4),
          denominator (getState(), IDs::TimeSigDenominator, getUndoManager(), 4)
    {
        Utility::writeBackDefaultValueIfNotThere (numerator);
        Utility::writeBackDefaultValueIfNotThere (denominator);
    }

    double barInTermsOfBeat (double beat) const noexcept
    {
        double currNumerator = numerator.get();
        return beat / currNumerator;
    }

    int barForBeat (double beat) const noexcept
    {
        return static_cast<int> (barInTermsOfBeat (beat));
    }

    double beatInBar (double beat) const noexcept
    {
        return std::fmod (beat, numerator.get());
    }

    double quarterNotesPerBeat() const noexcept
    {
        return 4.0 / denominator.get();
    }

    double beatForStartOfLastBarOfBeat (double beat) const noexcept
    {
        return beat - std::fmod (beat, numerator.get());
    }

    CachedValue<SPSCAtomicWrapper<int>> numerator;
    CachedValue<SPSCAtomicWrapper<int>> denominator;
};
