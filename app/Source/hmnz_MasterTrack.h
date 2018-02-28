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
    MasterTrack (const ValueTree& v, UndoManager* um, Edit* const edit);
    ~MasterTrack() = default;

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override {}
    void releaseResources() override {}
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override {}

    MidiMessageSequence createMetaEventsSequence() const noexcept;

    Tempo tempo;
    TimeSignature timeSignature;
    KeySignature keySignature;

private:
    Edit* const edit;
};
