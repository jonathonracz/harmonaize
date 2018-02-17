/*
  ==============================================================================

    hmnz_PositionedAudioMidiSource.h
    Created: 15 Feb 2018 1:22:22pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class PositionedAudioMidiSource
{
public:
    virtual ~PositionedAudioMidiSource() {}

    virtual void prepareToPlay (int samplesPerBlockExpected, double sampleRate) = 0;
    virtual void releaseResources() = 0;
    virtual void getNextAudioBlockWithInputs (AudioBuffer<float>& audioBuffer,
        const MidiBuffer& incomingMidiBuffer,
        const AudioPlayHead::CurrentPositionInfo& positionInfo) = 0;
};
