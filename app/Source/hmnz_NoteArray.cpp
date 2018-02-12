/*
  ==============================================================================

    hmnz_NoteArray.cpp
    Created: 28 Jan 2018 3:42:00pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_NoteArray.h"
#include "hmnz_Clip.h"

NoteArray::NoteArray (const ValueTree& v, UndoManager* um)
    : ValueTreeObjectArray (v, um)
{
    addObjects();
}

Note* NoteArray::createNewObject (const ValueTree& v, UndoManager* um)
{
    return new Note (v, um);
}
