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
                  public AsyncUpdater
{
public:
    enum State : int
    {
        stopped,
        playing
    };

    Transport (Edit* const edit);
    ~Transport();

    void setNextReadPosition (int64 newPosition) override;
    int64 getNextReadPosition() const override;
    int64 getTotalLength() const override;

private:
    Edit* const edit;

    // Audio bits
    AudioSourcePlayer output;
    AudioTransportSource transportSource;
    int64 readPosition;
    std::atomic<double> desiredReadPositionTime;
    std::atomic<double> readPositionTime;

    // MIDI bits
    MidiMessageCollector midiMessageCollector;
    MidiKeyboardState keyboardState;

    // PositionableAudioSource overrides
    bool isLooping() const override { return false; }
    void setLooping (bool shouldLoop) override {}
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;

    void changeListenerCallback (ChangeBroadcaster* source) override;
    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
    void handleAsyncUpdate() override;

    CachedValue<SPSCRelaxedLoadAtomicWrapper<double>> originTime;
    CachedValue<SPSCRelaxedLoadAtomicWrapper<double>> endTime;
    CachedValue<SPSCRelaxedLoadAtomicWrapper<double>> sampleRate;

    CachedValue<double> playPositionTime;
    CachedValue<int> playState;
};
