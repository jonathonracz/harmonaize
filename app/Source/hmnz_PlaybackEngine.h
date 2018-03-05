/*
  ==============================================================================

    hmnz_PlaybackEngine.h
    Created: 23 Feb 2018 3:14:33pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "hmnz_GenericValueTreeObjectArray.h"

class Edit;
class Track;

class PlaybackEngine    : public AudioSource,
                          public AudioPlayHead,
                          public ChangeListener,
                          public AsyncUpdater,
                          public ValueTree::Listener,
                          public ValueTreeObjectArray<Track, CriticalSection>::Listener
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

    Edit* getEdit() const noexcept;
    void setEdit (Edit* edit) noexcept;

    bool canControlTransport() noexcept override { return true; }

    double getActiveSampleRate() const noexcept { return activeSampleRate; }
    int getActiveSamplesPerBlockExpected() const noexcept { return activeSamplesPerBlockExpected; }
    uint64 getRecordOperationID() const noexcept { return recordOperationID.load (std::memory_order_acquire); }

    // Note that these should ONLY be called on the audio thread.
    bool getCurrentPosition (AudioPlayHead::CurrentPositionInfo& result) noexcept override;

    // These should ONLY be called on the message thread.
    void transportPlay (bool shouldStartPlaying) noexcept override;
    void transportRecord (bool shouldStartRecording) noexcept override;
    void transportRewind() noexcept override;

    void setPositionSample (int64 sample) noexcept;
    void setPositionSecond (double second) noexcept;
    void setPositionBeat (double beat) noexcept;

    std::mutex& getCallbackLock() noexcept { return callbackLock; }

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

    // AudioSource overrides
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) noexcept override;
    void releaseResources() noexcept override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) noexcept override;

    void changeListenerCallback (ChangeBroadcaster* source) noexcept override;

    void handleAsyncUpdate() noexcept override;

    void valueTreePropertyChanged (ValueTree& tree, const Identifier& identifier) noexcept override;
    void valueTreeChildAdded (ValueTree&, ValueTree&) override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

    void objectAdded (Track* track, ValueTreeObjectArray<Track, CriticalSection>* array) noexcept override;
};
