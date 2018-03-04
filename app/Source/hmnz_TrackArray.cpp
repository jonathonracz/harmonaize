/*
  ==============================================================================

    hmnz_TrackArray.cpp
    Created: 28 Jan 2018 3:16:29pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_TrackArray.h"
#include "hmnz_Edit.h"

TrackArray::TrackArray (Edit* const _edit)
    : ValueTreeObjectArray (_edit->getState(), _edit->getUndoManager()),
      edit (_edit)
{
    addObjects();
}

Track* TrackArray::createNewObject (const ValueTree& v, UndoManager* um)
{
    jassert (um == edit->getUndoManager());
    Track* newTrack = new Track (v, um, edit);
    return newTrack;
}
