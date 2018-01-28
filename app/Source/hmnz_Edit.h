/*
  ==============================================================================

    hmnz_Edit.h
    Created: 22 Jan 2018 12:56:24am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"
#include "hmnz_ValueTreeObjectArray.h"
#include "hmnz_MasterTack.h"
#include "hmnz_Transport.h"
#include "hmnz_Track.h"

class MasterTrack;
class Transport;

/**
    Represents an active edit (also known as a project).
*/
class Edit  : public ValueTreeObject<IDs::Edit>,
              public Component,
              public PositionableAudioSource
{
public:
    Edit (const ValueTree& v);

    static ValueTree createSkeletonEdit();

private:
    UndoManager undoManager;
    std::unique_ptr<MasterTrack> masterTrack;
    std::unique_ptr<Transport> transport;
    ValueTreeObjectArray<Track> tracks;

    std::mutex playbackLock;

    Value originTime;
    Value endTime;
    Value pulsesPerQuarterNote;
    Value sampleRate;

    int64 currentPlaybackPosition;

    jcf::ValueTreeDebugger stateDebugger;

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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Edit)
};
