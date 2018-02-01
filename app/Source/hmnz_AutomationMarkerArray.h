/*
  ==============================================================================

    hmnz_AutomationMarkerArray.h
    Created: 31 Jan 2018 6:22:55pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObjectArray.h"
#include "hmnz_AutomationMarker.h"

/**
    Used internally by the Automation class to manage all automation markers.
*/
template<typename ValueType>
class AutomationMarkerArray :   public ValueTreeObjectArray<AutomationMarker<ValueType>>
{
public:
    AutomationMarkerArray (const ValueTree& parentTree, UndoManager* um);

private:
    AutomationMarker<ValueType>* createNewObject (const ValueTree& v, UndoManager* um) override
    {
        return new AutomationMarker<ValueType> (v, um);
    }
};
