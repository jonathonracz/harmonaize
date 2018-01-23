/*
  ==============================================================================

    hmnz_Clip.h
    Created: 22 Jan 2018 12:57:58am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObjectArray.h"
#include "hmnz_Note.h"

class Clip  : public ValueTreeObject<IDs::Clip>
{
public:
    Clip (const ValueTree& v, UndoManager* um)
        : ValueTreeObject (v, um), notes (v, um) {}

private:
    ValueTreeObjectArray<Note> notes;
};
