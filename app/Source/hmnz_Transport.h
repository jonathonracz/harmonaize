/*
  ==============================================================================

    hmnz_Transport.h
    Created: 26 Jan 2018 3:10:54pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"
#include "hmnz_CacheValueWrappers.h"

class Edit;

class Transport : public ValueTreeObject<IDs::Transport>,
                  public AudioSource,
                  public AudioPlayHead,
                  public AsyncUpdater,
                  public ChangeListener
{
public:
    enum State : int
    {
        stopped,
        playing
    };

    Transport (const ValueTree& v, UndoManager* um, Edit* const edit);
    ~Transport();

    bool canControlTransport() override { return true; }

    // Note that these should ONLY be called on the audio thread.
    bool getCurrentPosition (AudioPlayHead::CurrentPositionInfo& result) override;

    // These should ONLY be called on the message thread.
    void transportPlay (bool shouldStartPlaying) override;
    void transportRecord (bool shouldStartRecording) override;
    void transportRewind() override;

    void setPositionSample (int64 sample) noexcept;
    void setPositionSecond (double second) noexcept;

    std::mutex& getCallbackLock() { return callbackLock; }
    MidiKeyboardState& getMidiKeyboardState() { return keyboardState; }

    int getActiveSamplesPerBlockExpected() const noexcept { return activeSamplesPerBlockExpected; }
    double getActiveSampleRate() const noexcept { return activeSampleRate; }
    uint64 getActiveRecordOperationID() const noexcept { return recordOperationID.load (std::memory_order_acquire); }

    CachedValue<double> playHeadTime;
    CachedValue<double> playHeadBeat;
    CachedValue<double> playHeadTempo;
    CachedValue<int> playHeadTimeSigNumerator;
    CachedValue<int> playHeadTimeSigDenominator;
    CachedValue<int> playHeadKeySigNumSharpsOrFlats;
    CachedValue<bool> playHeadKeySigIsMinor;

    CachedValue<SPSCAtomicWrapper<int>> playState;
    CachedValue<SPSCAtomicWrapper<bool>> recordEnabled;
    CachedValue<SPSCAtomicWrapper<double>> pulsesPerQuarterNote;

    Edit* const edit;

private:
    // Audio bits
    AudioSourcePlayer output;
    std::atomic<int64> readPosition;
    std::atomic<double> readPositionTime;
    std::atomic<double> readPositionBeat;
    std::atomic<double> readPositionTempo;
    std::atomic<int> readPositionTimeSigNumerator;
    std::atomic<int> readPositionTimeSigDenominator;
    std::atomic<int> readPositionKeySigNumSharpsOrFlats;
    std::atomic<bool> readPositionKeySigIsMinor;
    std::mutex callbackLock;

    int activeSamplesPerBlockExpected = -1;
    double activeSampleRate = 0.0;
    std::atomic<uint64> recordOperationID = { 0 };

    // MIDI bits
    MidiMessageCollector midiMessageCollector;
    MidiKeyboardState keyboardState;

    // PositionableAudioSource overrides
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
    void handleAsyncUpdate() override;

    void changeListenerCallback (ChangeBroadcaster*) override;

    AudioPlayHead::CurrentPositionInfo currentPositionInfo;

    CachedValue<SPSCAtomicWrapper<double>> loopStartBeat;
    CachedValue<SPSCAtomicWrapper<double>> loopEndBeat;
    CachedValue<SPSCAtomicWrapper<bool>> loopEnabled;

    JUCE_DECLARE_WEAK_REFERENCEABLE (Transport)
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Transport)
};
