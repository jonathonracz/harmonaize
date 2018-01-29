/*
  ==============================================================================

    hmnz_Note.cpp
    Created: 28 Jan 2018 1:44:47pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_Note.h"
#include "hmnz_Clip.h"
#include "hmnz_ValueTreeObject.h"

Note::Note (const ValueTree& v, UndoManager* um, const Clip* _owner)
    : ValueTreeObject (v, um), owner (_owner)
{
}
