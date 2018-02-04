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
#include "hmnz_TempoMap.h"

class Tempo : ValueTreeObject<IDs::Tempo>
{
public:
    Tempo (const ValueTree& v, UndoManager* um)
        : ValueTreeObject (v, um), automation (Automation<double>::createDefaultState(), um),
          tempoMap (ValueTreeObject<IDs::TempoMap>::createDefaultState(), automation)
    {
        getState().addChild (automation.getState(), -1, nullptr);
        getState().addChild (tempoMap.getState(), -1, nullptr);
    }

    Automation<double> automation;
    TempoMap tempoMap;
};
