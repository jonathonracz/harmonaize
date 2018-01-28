/*
  ==============================================================================

    hmnz_Track.h
    Created: 22 Jan 2018 12:56:50am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"
#include "hmnz_ValueTreeObjectArray.h"
#include "hmnz_Clip.h"

class Track : public ValueTreeObject<IDs::Track>,
              public PositionableAudioSource
{
public:
    Track (const ValueTree& v, UndoManager* um);
    virtual ~Track() = default;

private:
    ValueTreeObjectArray<Clip> clips;

    // PositionableAudioSource methods
    void setNextReadPosition (int64 newPosition) override;
    int64 getNextReadPosition() const override;
    int64 getTotalLength() const override;
    bool isLooping() const override { return false; }
    void setLooping (bool shouldLoop) override {}

    // AudioSource methods
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
};
