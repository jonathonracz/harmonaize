/*
  ==============================================================================

    hmnz_ClipArray.cpp
    Created: 28 Jan 2018 3:41:32pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_ClipArray.h"
#include "hmnz_Track.h"

ClipArray::ClipArray (const Track* _owner)
    : ValueTreeObjectArray (_owner->getState(), _owner->getUndoManager()),
      owner (_owner)
{
    addObjects();
}

Clip* ClipArray::createNewObject (const ValueTree& v, UndoManager* um)
{
    jassert (owner->getState() == v && um == owner->getUndoManager());
    return new Clip (v, um, owner);
}
