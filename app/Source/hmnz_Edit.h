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
#include "hmnz_MasterTrack.h"
#include "hmnz_Transport.h"
#include "hmnz_Track.h"
#include "hmnz_CacheValueWrappers.h"
#include "hmnz_AutomationMarker.h"
#include "hmnz_PositionedAudioMidiSource.h"

/**
    Represents an active edit (also known as a project).
*/
class Edit  : public ValueTreeObject<IDs::Edit>,
              public PositionedAudioMidiSource
{
public:
    Edit (const ValueTree& v);
    ~Edit();

    // AudioSource methods
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlockWithInputs (AudioBuffer<float>& audioBuffer,
        const MidiBuffer& incomingMidiBuffer,
        const AudioPlayHead::CurrentPositionInfo& positionInfo) override;

    MidiFile exportToMidi() const noexcept;
    void importFromMidi (const MidiFile& midiFile, int trackOffset, double timeOffset) noexcept;

    MasterTrack masterTrack;
    Transport transport;
    TrackArray tracks;

private:
    UndoManager undoManager;
    jcf::ValueTreeDebugger stateDebugger;

    void convertTimestampsFromBeatsToTicks (MidiMessageSequence& sequence) const noexcept;

    JUCE_DECLARE_WEAK_REFERENCEABLE (Edit)
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Edit)
};
