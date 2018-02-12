/*
  ==============================================================================

    hmnz_NoteList.h
    Created: 9 Feb 2018 2:04:08pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"
#include "hmnz_NoteArray.h"

class NoteList  : public ValueTreeObject<IDs::NoteList>
{
public:
    NoteList (const ValueTree& v, UndoManager* um)
        : ValueTreeObject (v, um), notes (v, um)
    {
    }

    NoteArray notes;
};
