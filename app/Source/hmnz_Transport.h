/*
  ==============================================================================

    hmnz_Transport.h
    Created: 26 Jan 2018 3:10:54pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"

class Transport : public ValueTreeObject<IDs::Transport>
{
public:
    Transport (const ValueTree& v, UndoManager* um)
        : ValueTreeObject (v, um) {}

    AudioTransportSource transportSource;
};
