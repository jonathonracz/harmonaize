/*
  ==============================================================================

    hmnz_Tempo.h
    Created: 1 Feb 2018 12:37:20am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"
#include "hmnz_Automation.h"

class Tempo : ValueTreeObject<IDs::Tempo>
{
public:
    Tempo (const ValueTree& v, UndoManager* um)
        : ValueTreeObject (v, um)
    {
    }

    Automation<double> automation;
};
