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
#include "hmnz_PositionedAudioMidiSource.h"
#include "hmnz_CacheValueWrappers.h"

class Edit;

class Track : public ValueTreeObject<IDs::Track>,
              public PositionedAudioMidiSource
{
public:
    Track (const ValueTree& v, UndoManager* um, Edit* const edit);

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate);
    void releaseResources();
    void getNextAudioBlockWithInputs (AudioBuffer<float>& audioBuffer,
        const MidiBuffer& incomingMidiBuffer,
        const AudioPlayHead::CurrentPositionInfo& positionInfo);

    CachedValue<String> name;
    CachedValue<Colour> color;
    CachedValue<Identifier> type;
    CachedValue<int> midiTrack;
    CachedValue<SPSCAtomicWrapper<bool>> recordArmed;

    Edit* const edit;

private:
    AudioBuffer<float> audioBuffer;
    MidiBuffer midiBuffer;
    ClipList clipList;

    Clip* currentRecordTarget = nullptr;
};
