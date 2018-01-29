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

Transport::Transport (Edit* const _source)
    : ValueTreeObject (_source->getState().getChildWithName(IDs::Transport), _source->getUndoManager()),
      source (_source),
      playPositionTime (getState(), IDs::TransportProps::PlayPositionTime, getUndoManager(), 0.0),
      playState (getState(), IDs::TransportProps::PlayState, getUndoManager(), State::stopped)
{
    HarmonaizeApplication::getDeviceManager().addAudioCallback (&output);
    output.setSource (&transportSource);
    transportSource.setSource (source);
    transportSource.addChangeListener (this);
    getState().addListener (this);
}

Transport::~Transport()
{
    HarmonaizeApplication::getDeviceManager().removeAudioCallback (&output);
}

void Transport::changeListenerCallback (ChangeBroadcaster* source)
{
    playPositionTime = transportSource.getCurrentPosition();
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
            transportSource.setPosition (playPositionTime);
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
    }
}
