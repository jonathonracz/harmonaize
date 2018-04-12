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
#include "hmnz_Genre.h"
#include "hmnz_PlaybackEngine.h"

class Edit;

class MasterTrack   : public ValueTreeObject<IDs::MasterTrack>,
                      public PlaybackEngine::PlaybackTarget
{
public:
    MasterTrack (const ValueTree& v, UndoManager* um);
    ~MasterTrack() = default;

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlockWithInputs (AudioBuffer<float>& audioBuffer,
        MidiBuffer& incomingMidiBuffer,
        PlaybackEngine& playbackSource) override;

    MidiMessageSequence createMetaEventsSequence() const;

    Tempo tempo;
    TimeSignature timeSignature;
    KeySignature keySignature;
    Genre genre;

    CachedValue<SPSCAtomicWrapper<bool>> metronomeEnabled;

private:
    MidiBuffer midiBuffer;
    sfzero::Synth metronome;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MasterTrack)
};
