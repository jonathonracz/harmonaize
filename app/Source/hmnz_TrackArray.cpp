/*
  ==============================================================================

    hmnz_TrackArray.cpp
    Created: 28 Jan 2018 3:16:29pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_TrackArray.h"
#include "hmnz_Edit.h"

TrackArray::TrackArray (const ValueTree& v, UndoManager* um, Edit* _edit)
    : HomogeneousValueTreeObjectArray (v, um), edit (_edit)
{
    addObjects();
}

Track* TrackArray::createNewObject (const ValueTree& v, UndoManager* um)
{
    jassert (um == edit->getUndoManager());
    return new Track (v, um, edit);
}
