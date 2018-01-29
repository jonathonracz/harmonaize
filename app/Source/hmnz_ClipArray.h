/*
  ==============================================================================

    hmnz_ClipArray.h
    Created: 28 Jan 2018 3:41:32pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_Clip.h"
#include "hmnz_ValueTreeObjectArray.h"

class ClipArray : public ValueTreeObjectArray<Clip>
{
public:
    ClipArray (Track* const owner);

private:
    Track* const owner;

    Clip* createNewObject (const ValueTree& v, UndoManager* um) override;
};
