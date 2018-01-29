/*
  ==============================================================================

    hmnz_Edit.h
    Created: 22 Jan 2018 12:56:24am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"
#include "hmnz_TrackArray.h"
#include "hmnz_MasterTack.h"
#include "hmnz_Transport.h"
#include "hmnz_Track.h"

/**
    Represents an active edit (also known as a project).
*/
class Edit  : public ValueTreeObject<IDs::Edit>,
              public Component,
              public PositionableAudioSource
{
public:
    Edit (const ValueTree& v);
    ~Edit();

    static ValueTree createSkeletonEdit();

    // PositionableAudioSource methods
    void setNextReadPosition (int64 newPosition) override;
    int64 getNextReadPosition() const override;
    int64 getTotalLength() const override;

private:
    UndoManager undoManager;
    std::unique_ptr<MasterTrack> masterTrack;
    std::unique_ptr<Transport> transport;
    TrackArray tracks;
    std::mutex playbackLock;
    int64 currentPlaybackPosition;
    jcf::ValueTreeDebugger stateDebugger;

    CachedValue<double> originTime;
    CachedValue<double> endTime;
    CachedValue<int> pulsesPerQuarterNote;
    CachedValue<double> sampleRate;

    // ValueTree methods
    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;

    // Component methods
    void resized() override;
    void paint (Graphics&) override;

    // PositionableAudioSource methods
    bool isLooping() const override { return false; }
    void setLooping (bool shouldLoop) override {}

    // AudioSource methods
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Edit)
};
