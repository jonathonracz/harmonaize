/*
  ==============================================================================

    hmnz_Track.h
    Created: 22 Jan 2018 12:56:50am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"
#include "hmnz_VariantConverters.h"
#include "hmnz_ClipList.h"

class Edit;

class Track : public ValueTreeObject<IDs::Track>,
              public AudioSource
{
public:
    Track (const ValueTree& v, UndoManager* um, Edit* const edit);

    // AudioSource methods
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;

    CachedValue<String> name;
    CachedValue<Colour> color;
    CachedValue<Identifier> type;
    CachedValue<int> MidiTrack;

    Edit* const edit;

private:
    AudioBuffer<float> audioBuffer;
    MidiBuffer midiBuffer;
    ClipList clipList;

    
};
