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
#include "hmnz_PlaybackEngine.h"
#include "hmnz_CacheValueWrappers.h"
#include "hmnz_PlaybackEngine.h"
#include "hmnz_SFZInstrumentBank.h"
#include "External/readerwriterqueue/readerwriterqueue.h"

class Edit;

class Track : public ValueTreeObject<IDs::Track>,
              public PlaybackEngine::PlaybackTarget,
              public AsyncUpdater,
              public ValueTree::Listener,
              private Timer
{
public:
    Track (const ValueTree& v, UndoManager* um, Edit& edit);

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlockWithInputs (AudioBuffer<float>& audioBuffer,
        MidiBuffer& incomingMidiBuffer,
        PlaybackEngine& playbackSource) override;

    MidiMessageSequence getMidiMessageSequence() const;
    void addMidiMessageSequenceAsClip (double start, double length, const MidiMessageSequence& sequence);

    void loadSFZInstrument (const File& sfzInstrument);

    CachedValue<String> name;
    CachedValue<Colour> color;
    CachedValue<Identifier> type;
    CachedValue<MinMaxConstrainerWrapper<int, 32, 128>> height;
    CachedValue<SPSCAtomicWrapper<bool>> recordArmed;

    Edit& edit;
    ClipList clipList;

    std::shared_ptr<double> loadProgress = std::make_shared<double> (-1.0);

private:
    SFZInstrumentBank::LoadFuture loadFuture;
    std::atomic<bool> isPrepared = { false };
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

    void updateMidiReadCache();
    void flushMidiWriteBackQueue();

    void handleAsyncUpdate() override;

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&) override;
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override;
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

    void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Track)
};
