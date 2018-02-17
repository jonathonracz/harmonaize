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
                  public ChangeListener,
                  public PositionableAudioSource,
                  public AudioPlayHead,
                  public AsyncUpdater
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
    int64 getNextReadPosition() const override;
    int64 getTotalLength() const override;
    double getCurrentPosition() const;
    double getLengthInSeconds() const;
    bool getCurrentPosition (AudioPlayHead::CurrentPositionInfo& result) override;

    // These should ONLY be called on the message thread.
    void transportPlay (bool shouldStartPlaying) override;
    void transportRecord (bool shouldStartRecording) override;
    void transportRewind() override;

    CachedValue<double> playHeadTime;
    CachedValue<double> playHeadBeat;
    CachedValue<double> playHeadTempo;
    CachedValue<int> playHeadTimeSigNumerator;
    CachedValue<int> playHeadTimeSigDenominator;
    CachedValue<int> playHeadKeySigNumSharpsOrFlats;
    CachedValue<bool> playHeadKeySigIsMinor;

    CachedValue<int> playState;
    CachedValue<SPSCAtomicWrapper<double>> sampleRate;

    std::mutex& getCallbackLock() { return callbackLock; }

    Edit* const edit;

private:
    // Audio bits
    AudioSourcePlayer output;
    AudioTransportSource transportSource;
    std::atomic<int64> readPosition;
    std::atomic<double> readPositionTime;
    std::atomic<double> readPositionBeat;
    std::atomic<double> readPositionTempo;
    std::atomic<int> readPositionTimeSigNumerator;
    std::atomic<int> readPositionTimeSigDenominator;
    std::atomic<int> readPositionKeySigNumSharpsOrFlats;
    std::atomic<bool> readPositionKeySigIsMinor;
    int currentSamplesPerBlockExpected = -1;
    std::mutex callbackLock;

    // MIDI bits
    MidiMessageCollector midiMessageCollector;
    MidiKeyboardState keyboardState;

    // PositionableAudioSource overrides
    void setNextReadPosition (int64 newPosition) override;
    bool isLooping() const override { return false; }
    void setLooping (bool shouldLoop) override {}
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;

    void changeListenerCallback (ChangeBroadcaster* source) override;
    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
    void handleAsyncUpdate() override;

    AudioPlayHead::CurrentPositionInfo currentPositionInfo;

    CachedValue<SPSCAtomicWrapper<double>> pulsesPerQuarterNote;
    CachedValue<SPSCAtomicWrapper<bool>> recordEnabled;
    CachedValue<SPSCAtomicWrapper<double>> loopStartBeat;
    CachedValue<SPSCAtomicWrapper<double>> loopEndBeat;
    CachedValue<SPSCAtomicWrapper<bool>> loopEnabled;

    JUCE_DECLARE_WEAK_REFERENCEABLE (Transport)
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Transport)
};
