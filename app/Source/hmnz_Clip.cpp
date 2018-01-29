/*
  ==============================================================================

    hmnz_Clip.cpp
    Created: 28 Jan 2018 1:45:02pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_Clip.h"
#include "hmnz_Track.h"

Clip::Clip (const ValueTree& v, UndoManager* um, const Track* _owner)
    : ValueTreeObject (v, um), owner (_owner), notes (this)
{
}
