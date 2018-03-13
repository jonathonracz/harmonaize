/*
  ==============================================================================

    hmnz_MasterTrack.h
    Created: 26 Jan 2018 4:00:25pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"
#include "hmnz_Tempo.h"
#include "hmnz_TimeSignature.h"
#include "hmnz_KeySignature.h"

class Edit;

class MasterTrack   : public ValueTreeObject<IDs::MasterTrack>,
                      public AudioSource
{
public:
    MasterTrack (const ValueTree& v, UndoManager* um);
    ~MasterTrack() = default;

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override {}
    void releaseResources() override {}
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override {}

    MidiMessageSequence createMetaEventsSequence() const;

    Tempo tempo;
    TimeSignature timeSignature;
    KeySignature keySignature;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MasterTrack)
};
