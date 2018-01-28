/*
  ==============================================================================

    hmnz_Clip.cpp
    Created: 28 Jan 2018 1:45:02pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_Clip.h"

Clip::Clip (const ValueTree& v, UndoManager* um)
    : ValueTreeObject (v, um), notes (v, um)
{
}
