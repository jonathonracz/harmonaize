/*
  ==============================================================================

    hmnz_NoteArray.h
    Created: 28 Jan 2018 3:42:00pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_Note.h"
#include "hmnz_ValueTreeObjectArray.h"

class NoteArray : public ValueTreeObjectArray<Note>
{
public:
    NoteArray (const ValueTree& v, UndoManager* um);

private:
    Note* createNewObject (const ValueTree& v, UndoManager* um) override;
};
