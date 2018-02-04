/*
  ==============================================================================

    hmnz_TempoMapIntervalArray.h
    Created: 4 Feb 2018 12:29:44pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObjectArray.h"
#include "hmnz_TempoMapInterval.h"

class TempoMapIntervalArray : public ValueTreeObjectArray<TempoMapInterval>
{
public:
    using ValueTreeObjectArray::ValueTreeObjectArray;

    TempoMapInterval* createNewObject (const ValueTree& v, UndoManager* um) override
    {
        return new TempoMapInterval (v, um);
    }
};
