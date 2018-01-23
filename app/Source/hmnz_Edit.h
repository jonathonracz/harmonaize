/*
  ==============================================================================

    hmnz_Edit.h
    Created: 22 Jan 2018 12:56:24am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"
#include "hmnz_ValueTreeObjectArray.h"

/**
    Represents an active edit (also known as a project).
*/
class Edit  : public ValueTreeObject<IDs::Edit>
{
public:
    Edit (const ValueTree& v)
        : ValueTreeObject (v, undoManager) {}

private:
    UndoManager undoManager;
};
