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

class Track;

class ClipArray : public ValueTreeObjectArray<Clip>
{
public:
    ClipArray (const ValueTree& v, UndoManager* um);

private:
    Clip* createNewObject (const ValueTree& v, UndoManager* um) override;
};
