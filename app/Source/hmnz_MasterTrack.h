/*
  ==============================================================================

    hmnz_MasterTrack.h
    Created: 26 Jan 2018 4:00:25pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"
#include "hmnz_CacheValueWrappers.h"
#include "hmnz_Tempo.h"

class Edit;

class MasterTrack   : public ValueTreeObject<IDs::MasterTrack>,
                      public AudioSource
{
public:
    MasterTrack (Edit* const edit);

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override {}
    void releaseResources() override {}
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override {}

    double getBeatsPerMinuteAtTime (double time);
    double getSecondsPerBeatAtTime (double time);
    double getBeatsAtTime (double time);

private:
    Edit* const edit;

    CachedValue<SPSCRelaxedLoadAtomicWrapper<double>> beatsPerMinute;
    CachedValue<SPSCRelaxedLoadAtomicWrapper<int>> timeSigNumerator;
    CachedValue<SPSCRelaxedLoadAtomicWrapper<int>> timeSigDenominator;
};
