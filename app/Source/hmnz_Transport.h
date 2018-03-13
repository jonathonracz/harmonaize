/*
  ==============================================================================

    hmnz_Transport.h
    Created: 26 Jan 2018 3:10:54pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"
#include "hmnz_CacheValueWrappers.h"

class Transport : public ValueTreeObject<IDs::Transport>
{
public:
    enum State : int
    {
        stopped,
        playing
    };

    Transport (const ValueTree& v);

    CachedValue<double> playHeadTime;
    CachedValue<double> playHeadBeat;
    CachedValue<double> playHeadTempo;
    CachedValue<int> playHeadTimeSigNumerator;
    CachedValue<int> playHeadTimeSigDenominator;
    CachedValue<int> playHeadKeySigNumSharpsOrFlats;
    CachedValue<bool> playHeadKeySigIsMinor;

    CachedValue<SPSCAtomicWrapper<int>> playState;
    CachedValue<SPSCAtomicWrapper<bool>> recordEnabled;

    static constexpr double pulsesPerQuarterNote = 960.0;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Transport)
};
