/*
  ==============================================================================

    hmnz_TrackArray.cpp
    Created: 28 Jan 2018 3:16:29pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_TrackArray.h"
#include "hmnz_Edit.h"

TrackArray::TrackArray (const Edit* _owner)
    : ValueTreeObjectArray (_owner->getState(), _owner->getUndoManager()),
      owner (_owner)
{
    addObjects();
}

Track* TrackArray::createNewObject (const ValueTree& v, UndoManager* um)
{
    jassert (owner->getState() == v && um == owner->getUndoManager());
    return new Track (v, um, owner);
}
