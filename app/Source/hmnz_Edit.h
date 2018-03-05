/*
  ==============================================================================

    hmnz_Edit.h
    Created: 22 Jan 2018 12:56:24am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"
#include "hmnz_TrackList.h"
#include "hmnz_MasterTrack.h"
#include "hmnz_Transport.h"
#include "hmnz_Track.h"
#include "hmnz_CacheValueWrappers.h"
#include "hmnz_PlaybackEngine.h"
#include "hmnz_ArrangementViewModel.h"

/**
    Represents an active edit (also known as a project).
*/
class Edit  : public ValueTreeObject<IDs::Edit>,
              public PlaybackEngine::PlaybackTarget
{
public:
    Edit (const ValueTree& v, bool useUndoManager = true);
    ~Edit();

    // AudioSource methods
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlockWithInputs (
        AudioBuffer<float>& audioBuffer,
        MidiBuffer& incomingMidiBuffer,
        PlaybackEngine& playbackSource) override;

    MidiFile exportToMidi() const noexcept;
    void importFromMidi (const MidiFile& midiFile, int trackOffset, double timeOffset) noexcept;

    std::mutex* getPlaybackLock () const noexcept { return playbackLock; }
    void setPlaybackLock (std::mutex* _playbackLock) noexcept { playbackLock = _playbackLock; }

    MidiKeyboardState& getMidiKeyboardState() noexcept { return keyboardState; }

    MasterTrack masterTrack;
    Transport transport;
    ArrangementViewModel arrangementViewModel;
    TrackList trackList;

private:
    std::unique_ptr<UndoManager> undoManager;
    jcf::ValueTreeDebugger stateDebugger;
    std::mutex* playbackLock;

    MidiKeyboardState keyboardState;

    void convertTimestampsFromBeatsToTicks (MidiMessageSequence& sequence) const noexcept;

    JUCE_DECLARE_WEAK_REFERENCEABLE (Edit)
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Edit)
};
