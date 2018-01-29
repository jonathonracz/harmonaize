/*
  ==============================================================================

    hmnz_Transport.h
    Created: 26 Jan 2018 3:10:54pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"

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

private:
    Edit* const edit;
    AudioSourcePlayer output;
    AudioTransportSource transportSource;
    int64 readPosition;
    std::atomic<double> desiredReadPositionTime;
    std::atomic<double> readPositionTime;
    std::mutex callbackLock;

    // PositionableAudioSource methods
    void setNextReadPosition (int64 newPosition) override;
    int64 getNextReadPosition() const override;
    int64 getTotalLength() const override;
    bool isLooping() const override { return false; }
    void setLooping (bool shouldLoop) override {}

    // AudioSource methods
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;

    void changeListenerCallback (ChangeBroadcaster* source) override;
    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
    void handleAsyncUpdate() override;

    CachedValue<double> playPositionTime;
    CachedValue<int> playState;
    CachedValue<double> sampleRate;
};
