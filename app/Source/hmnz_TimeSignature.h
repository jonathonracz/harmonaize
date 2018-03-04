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
    struct Snapshot
    {
        double barInTermsOfBeat (double beat) const noexcept
        {
            return (beat - startBeat) / numerator;
        }

        int barForBeat (double beat) const noexcept
        {
            return static_cast<int> (barInTermsOfBeat (beat));
        }

        double beatInBar (double beat) const noexcept
        {
            return std::fmod (beat - startBeat, numerator);
        }

        double quarterNotesPerBeat() const noexcept
        {
            return 4.0 / static_cast<double> (denominator);
        }

        double beatForStartOfLastBarOfBeat (double beat) const noexcept
        {
            return beat - startBeat - std::fmod (beat, numerator);
        }

        int numerator;
        int denominator;
        double startBeat;
        double snapshotTime;
    };

    TimeSignature (const ValueTree& v, UndoManager* um)
        : ValueTreeObject (v, um),
          numerator (getState(), IDs::TimeSignatureProps::Numerator, getUndoManager(), 4),
          denominator (getState(), IDs::TimeSignatureProps::Denominator, getUndoManager(), 4)
    {
    }

    int getNumeratorAtTime (double time) const noexcept
    {
        return numerator.get();
    }

    void setNumeratorAtTime (int newNumerator, double time) noexcept
    {
        return numerator.setValue (newNumerator, getUndoManager());
    }

    int getDenominatorAtTime (double time) const noexcept
    {
        return denominator.get();
    }

    void setDenominatorAtTime (int newDenominator, double time) noexcept
    {
        return denominator.setValue (newDenominator, getUndoManager());
    }

    Snapshot getTimeSignatureAtTime (double time) const noexcept
    {
        Snapshot ret;
        ret.numerator = numerator.get().getRelaxed();
        ret.denominator = denominator.get().getRelaxed();
        ret.startBeat = 0.0;
        std::atomic_thread_fence (std::memory_order_acquire);
        ret.snapshotTime = time;
        return ret;
    }

    void setTimeSignature (Snapshot timeSignature) noexcept
    {
        std::atomic_thread_fence (std::memory_order_release);
        numerator.get().setRelaxed (timeSignature.numerator);
        denominator.get().setRelaxed (timeSignature.denominator);
    }

private:
    CachedValue<SPSCAtomicWrapper<int>> numerator;
    CachedValue<SPSCAtomicWrapper<int>> denominator;
};
