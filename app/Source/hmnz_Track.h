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
#include "External/readerwriterqueue/readerwriterqueue.h"

class Edit;

class Track : public ValueTreeObject<IDs::Track>,
              public PositionedAudioMidiSource,
              public AsyncUpdater
{
public:
    Track (const ValueTree& v, UndoManager* um, Edit* const edit);

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlockWithInputs (AudioBuffer<float>& audioBuffer,
        const MidiBuffer& incomingMidiBuffer,
        const AudioPlayHead::CurrentPositionInfo& positionInfo) override;

    MidiMessageSequence getMidiMessageSequence() const noexcept;
    void addMidiMessageSequenceAsClip (double start, double length, const MidiMessageSequence& sequence) noexcept;

    CachedValue<String> name;
    CachedValue<Colour> color;
    CachedValue<Identifier> type;
    CachedValue<SPSCAtomicWrapper<bool>> recordArmed;

    Edit* const edit;
    ClipList clipList;

private:
    AudioBuffer<float> audioBuffer;
    MidiBuffer midiBuffer;

    MidiMessageSequence midiReadCache;

    struct RecordedMidiMessage
    {
        uint64 recordSessionID;
        MidiMessage message;
    };
    moodycamel::ReaderWriterQueue<RecordedMidiMessage> midiWriteBackQueue;
    uint64 currentRecordSessionID = 0;
    WeakReference<Clip> currentRecordClip;

    sfzero::Synth synthesizer;

    void updateMidiReadCache() noexcept;
    void flushMidiWriteBackQueue() noexcept;

    void handleAsyncUpdate() override;

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&) override;
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override;
};
