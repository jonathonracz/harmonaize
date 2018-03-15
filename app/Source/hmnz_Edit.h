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
    Edit (const ValueTree& v, UndoManager* um);
    ~Edit();

    // AudioSource methods
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlockWithInputs (
        AudioBuffer<float>& audioBuffer,
        MidiBuffer& incomingMidiBuffer,
        PlaybackEngine& playbackSource) override;

    MidiFile exportToMidi() const;
    void importFromMidi (const MidiFile& midiFile, int trackOffset, double timeOffset);

    MidiKeyboardState& getMidiKeyboardState() { return keyboardState; }

    void saveState();
    void newProject();
    File openProject();
    void changeFile(File file);

private:
    JUCE_DECLARE_WEAK_REFERENCEABLE (Edit)

public:
    MasterTrack masterTrack;
    Transport transport;
    ArrangementViewModel arrangementViewModel;
    TrackList trackList;

private:
    MidiKeyboardState keyboardState;

    File state;

    void convertTimestampsFromBeatsToTicks (MidiMessageSequence& sequence) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Edit)
};
