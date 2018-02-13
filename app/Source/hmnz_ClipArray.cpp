/*
  ==============================================================================

    hmnz_ClipArray.cpp
    Created: 28 Jan 2018 3:41:32pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_ClipArray.h"
#include "hmnz_Track.h"

ClipArray::ClipArray (const ValueTree& v, UndoManager* um, Track* const _track)
    : ValueTreeObjectArray (v, um), track (_track)
{
    addObjects();
}

Clip* ClipArray::createNewObject (const ValueTree& v, UndoManager* um)
{
    return new Clip (v, um, track);
}
