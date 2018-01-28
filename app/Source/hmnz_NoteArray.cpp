/*
  ==============================================================================

    hmnz_NoteArray.cpp
    Created: 28 Jan 2018 3:42:00pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_NoteArray.h"
#include "hmnz_Clip.h"

NoteArray::NoteArray (const Clip* _owner)
    : ValueTreeObjectArray (_owner->getState(), _owner->getUndoManager()),
      owner (_owner)
{
    addObjects();
}

Note* NoteArray::createNewObject (const ValueTree& v, UndoManager* um)
{
    jassert (um == owner->getUndoManager());
    return new Note (v, um, owner);
}
