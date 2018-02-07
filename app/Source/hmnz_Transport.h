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

private:
    Edit* const edit;

    // Audio bits
    AudioSourcePlayer output;
    AudioTransportSource transportSource;
    int64 readPosition;
    std::atomic<double> desiredReadPositionTime;
    std::atomic<double> readPositionTime;
    std::atomic<double> readPositionBeat;

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

    CachedValue<SPSCRelaxedLoadAtomicWrapper<double>> pulsesPerQuarterNote;
    CachedValue<SPSCRelaxedLoadAtomicWrapper<double>> sampleRate;

    CachedValue<double> playPositionTime;
    CachedValue<double> playPositionBeat;
    CachedValue<int> playState;
};
