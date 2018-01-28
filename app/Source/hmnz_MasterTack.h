/*
  ==============================================================================

    hmnz_MasterTack.h
    Created: 26 Jan 2018 4:00:25pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"

class MasterTrack   : public ValueTreeObject<IDs::MasterTrack>
{
public:
    MasterTrack (const ValueTree& v, UndoManager* um)
        : ValueTreeObject (v, um) {}
};
