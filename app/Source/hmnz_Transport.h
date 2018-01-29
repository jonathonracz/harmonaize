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
                  public ChangeListener
{
public:
    enum State : int
    {
        stopped,
        playing
    };

    Transport (Edit* const source);
    ~Transport();

private:
    Edit* const source;
    AudioSourcePlayer output;
    AudioTransportSource transportSource;

    void changeListenerCallback (ChangeBroadcaster* source) override;
    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;

    CachedValue<double> playPositionTime;
    CachedValue<int> playState;
};
