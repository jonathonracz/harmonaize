/*
  ==============================================================================

    hmnz_PlaybackEngine.h
    Created: 23 Feb 2018 3:14:33pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "hmnz_HomogeneousValueTreeObjectArray.h"

class Edit;
class Track;

class PlaybackEngine    : public AudioSource,
                          public AudioPlayHead,
                          public ChangeListener,
                          public AsyncUpdater,
                          public ValueTree::Listener,
                          public GenericHomogeneousValueTreeObjectArray<Track, CriticalSection>::Listener
{
public:
    struct PlaybackTarget
    {
        virtual ~PlaybackTarget() = default;
        virtual void prepareToPlay (int samplesPerBlockExpected, double sampleRate) = 0;
        virtual void releaseResources() = 0;
        virtual void getNextAudioBlockWithInputs (AudioBuffer<float>& audioBuffer,
            MidiBuffer& incomingMidiBuffer,
            PlaybackEngine& playbackSource) = 0;
    };

    PlaybackEngine();
    ~PlaybackEngine();

    Edit* getEdit() const;
    void setEdit (Edit* edit);

    bool canControlTransport() override { return true; }

    double getActiveSampleRate() const { return activeSampleRate; }
    int getActiveSamplesPerBlockExpected() const { return activeSamplesPerBlockExpected; }
    uint64 getRecordOperationID() const { return recordOperationID.load (std::memory_order_acquire); }

    // Note that these should ONLY be called on the audio thread.
    bool getCurrentPosition (AudioPlayHead::CurrentPositionInfo& result) override;

    // These should ONLY be called on the message thread.
    void transportPlay (bool shouldStartPlaying) override;
    void transportRecord (bool shouldStartRecording) override;
    void transportRewind() override;

    void setPositionSample (int64 sample);
    void setPositionSecond (double second);
    void setPositionBeat (double beat);

    std::mutex& getCallbackLock() { return callbackLock; }

private:
    struct CurrentPositionInfo  : public AudioPlayHead::CurrentPositionInfo
    {
        double timeInBeats;
        int keySigNumSharpsOrFlats;
        bool keySigIsMinor;
    };

    WeakReference<Edit> edit;
    std::atomic<int64> readPosition = { 0 };
    AudioSourcePlayer output;
    std::mutex callbackLock;
    std::atomic<CurrentPositionInfo> currentPositionInfo;
    std::atomic<bool> playHeadPositionChanged = { false };

    int activeSamplesPerBlockExpected = 0;
    double activeSampleRate = 0.0;
    std::atomic<uint64> recordOperationID = { 0 };

    // MIDI bits
    MidiMessageCollector midiMessageCollector;
    MidiBuffer midiStopBuffer;

    CurrentPositionInfo updatePositionInfo();

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;

    void changeListenerCallback (ChangeBroadcaster* source) override;

    void handleAsyncUpdate() override;

    void valueTreePropertyChanged (ValueTree& tree, const Identifier& identifier) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&) override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

    void objectAdded (Track* track, int, HomogeneousValueTreeObjectArray<Track, CriticalSection>*) override;
};
