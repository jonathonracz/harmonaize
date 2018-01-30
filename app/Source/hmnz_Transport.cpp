/*
  ==============================================================================

    hmnz_Transport.cpp
    Created: 28 Jan 2018 11:25:50pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_Transport.h"
#include "hmnz_Edit.h"
#include "hmnz_Application.h"

Transport::Transport (Edit* const _edit)
    : ValueTreeObject (_edit->getState(), _edit->getUndoManager()),
      edit (_edit),
      desiredReadPositionTime (0.0f),
      originTime (getState(), IDs::EditProps::OriginTime, nullptr),
      endTime (getState(), IDs::EditProps::EndTime, nullptr),
      sampleRate (getState(), IDs::EditProps::SampleRate, nullptr),
      playPositionTime (getState(), IDs::TransportProps::PlayPositionTime, nullptr, 0.0),
      playState (getState(), IDs::TransportProps::PlayState, nullptr, State::stopped)
{
    playPositionTime.get();
    playState.get();

    transportSource.setSource (this, 0, nullptr, sampleRate.get());
    std::atomic_thread_fence (std::memory_order_acquire);
    output.setSource (&transportSource);
    HarmonaizeApplication::getDeviceManager().addAudioCallback (&output);

    keyboardState.addListener (&midiMessageCollector);
    String firstMidiInput = MidiInput::getDevices()[0];
    if (firstMidiInput.isNotEmpty())
        HarmonaizeApplication::getDeviceManager().addMidiInputCallback (firstMidiInput, &midiMessageCollector);

    transportSource.addChangeListener (this);
    getState().addListener (this);
    transportSource.start();
}

Transport::~Transport()
{
    HarmonaizeApplication::getDeviceManager().removeAudioCallback (&output);
    output.setSource (nullptr);
    transportSource.setSource (nullptr);
}

void Transport::setNextReadPosition (int64 newPosition)
{
    readPosition = newPosition;
}

int64 Transport::getNextReadPosition() const
{
    return readPosition;
}

int64 Transport::getTotalLength() const
{
    int64 totalLength = (endTime.get() - originTime.get()) * sampleRate.get();
    std::atomic_thread_fence (std::memory_order_acquire);
    return totalLength;
}

void Transport::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    edit->prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void Transport::releaseResources()
{
    edit->releaseResources();
}

void Transport::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    readPositionTime.store (transportSource.getCurrentPosition(), std::memory_order_release);
    triggerAsyncUpdate();
    edit->getNextAudioBlock (bufferToFill);

    double loadedDesiredReadPositionTime = desiredReadPositionTime.load (std::memory_order_acquire);
    if (loadedDesiredReadPositionTime != std::numeric_limits<double>::min())
    {
        transportSource.setPosition (loadedDesiredReadPositionTime);
        desiredReadPositionTime.store (std::numeric_limits<double>::min());
    }
    else
    {
        setNextReadPosition (getNextReadPosition() + bufferToFill.numSamples);
    }
}

void Transport::changeListenerCallback (ChangeBroadcaster* source)
{
    if (transportSource.isPlaying())
        playState = State::playing;
    else
        playState = State::stopped;
}

void Transport::valueTreePropertyChanged (ValueTree& tree, const Identifier& identifier)
{
    if (tree == getState())
    {
        if (identifier == playPositionTime.getPropertyID())
        {
            desiredReadPositionTime.store (playPositionTime, std::memory_order_release);
        }
        else if (identifier == playState.getPropertyID())
        {
            switch (playState)
            {
                case playing: transportSource.start(); break;
                case stopped: transportSource.stop(); break;
                default: jassertfalse;
            }
        }
        else if (identifier == sampleRate.getPropertyID())
        {
            bool wasPlaying = (playState == State::playing);
            transportSource.stop();
            transportSource.setSource (this, 0, nullptr, sampleRate.get());
            std::atomic_thread_fence (std::memory_order_acquire);
            if (wasPlaying)
                transportSource.start();
        }
    }
}

void Transport::handleAsyncUpdate()
{
    getState().setPropertyExcludingListener (this, playPositionTime.getPropertyID(), readPositionTime.load (std::memory_order_acquire), getUndoManager());
}
