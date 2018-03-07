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

        double barForBeat (double beat) const noexcept
        {
            return std::floor (barInTermsOfBeat (beat));
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

        const int numerator;
        const int denominator;
        const double startBeat;
        const double snapshotTime;
    };

    TimeSignature (const ValueTree& v, UndoManager* um)
        : ValueTreeObject (v, um),
          numerator (getState(), IDs::TimeSignatureProps::Numerator, getUndoManager(), 4),
          denominator (getState(), IDs::TimeSignatureProps::Denominator, getUndoManager(), 4)
    {
    }

    int getNumeratorAtBeat (double time) const noexcept
    {
        return numerator.get();
    }

    void setNumeratorAtBeat (int newNumerator, double time) noexcept
    {
        return numerator.setValue (newNumerator, getUndoManager());
    }

    int getDenominatorAtBeat (double time) const noexcept
    {
        return denominator.get();
    }

    void setDenominatorAtBeat (int newDenominator, double time) noexcept
    {
        return denominator.setValue (newDenominator, getUndoManager());
    }

    double barInTermsOfBeat (double beat) const noexcept
    {
        return beat / numerator.get();
    }

    double barForBeat (double beat) const noexcept
    {
        return std::floor (barInTermsOfBeat (beat));
    }

    double beatInBar (double beat) const noexcept
    {
        return std::fmod (beat, numerator.get());
    }

    double quarterNotesPerBeat() const noexcept
    {
        return 4.0 / static_cast<double> (denominator.get());
    }

    double beatForStartOfLastBarOfBeat (double beat) const noexcept
    {
        return beat - std::fmod (beat, numerator.get());
    }

    Snapshot getTimeSignatureAtBeat (double beat) const noexcept
    {
        Snapshot ret {
            numerator.get().getRelaxed(),
            denominator.get().getRelaxed(),
            0.0,
            beat
        };
        std::atomic_thread_fence (std::memory_order_acquire);
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
