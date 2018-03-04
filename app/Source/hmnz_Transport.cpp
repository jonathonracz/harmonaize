/*
  ==============================================================================

    hmnz_Transport.cpp
    Created: 28 Jan 2018 11:25:50pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_Transport.h"

Transport::Transport (const ValueTree& v)
    : ValueTreeObject (v, nullptr),
      playHeadTime (getState(), IDs::TransportProps::PlayHeadTime, nullptr, 0.0),
      playHeadBeat (getState(), IDs::TransportProps::PlayHeadBeat, nullptr, 0.0),
      playHeadTempo (getState(), IDs::TransportProps::PlayHeadTempo, nullptr, 120.0),
      playHeadTimeSigNumerator (getState(), IDs::TransportProps::PlayHeadTimeSigNumerator, nullptr, 4),
      playHeadTimeSigDenominator (getState(), IDs::TransportProps::PlayHeadTimeSigDenominator, nullptr, 4),
      playHeadKeySigNumSharpsOrFlats (getState(), IDs::TransportProps::PlayHeadKeySigNumSharpsOrFlats, nullptr, 0),
      playHeadKeySigIsMinor (getState(), IDs::TransportProps::PlayHeadKeySigIsMinor, nullptr, false),
      playState (getState(), IDs::TransportProps::PlayState, nullptr, State::stopped),
      recordEnabled (getState(), IDs::TransportProps::RecordEnabled, nullptr, false)
{
}
