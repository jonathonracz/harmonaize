/*
  ==============================================================================

    hmnz_Track.h
    Created: 22 Jan 2018 12:56:50am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObjectArray.h"
#include "hmnz_Clip.h"

class Track : public ValueTreeObject<IDs::Track>
{
public:
    Track (const ValueTree& v, UndoManager* um)
        : ValueTreeObject (v, um), clips (v, um) {}

private:
    ValueTreeObjectArray<Clip> clips;
};
