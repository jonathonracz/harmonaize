/*
  ==============================================================================

    hmnz_Note.h
    Created: 23 Jan 2018 1:41:45am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"

class Note  : public ValueTreeObject<IDs::Note>
{
public:
    Note (const ValueTree& v, UndoManager* um)
        : ValueTreeObject (v, um) {}
};
