/*
  ==============================================================================

    hmnz_AutomationMarker.h
    Created: 31 Jan 2018 5:00:58pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include <jcf_advanced_leak_detector/jcf_advanced_leak_detector.h>
#include "hmnz_ValueTreeObject.h"
#include "hmnz_CacheValueWrappers.h"

/**
    Represents a single marker in an automation lane's beat map.

    This isn't intended to be used anywhere outside an Automation object.
*/
template<typename ValueType>
class AutomationMarker  : public ValueTreeObject<IDs::AutomationMarker>
{
public:
    enum Type   : int
    {
        origin,
        linear,
        step
    };

    AutomationMarker (const ValueTree& v, UndoManager* um)
        : ValueTreeObject (v, um),
          // Due to sibling constraints and other factors, undo is handled by the owner Automation object.
          time (getState(), IDs::AutomationMarkerProps::Time, nullptr),
          value (getState(), IDs::AutomationMarkerProps::Value, nullptr),
          type (getState(), IDs::AutomationMarkerProps::Type, nullptr)
    {
        static_assert(std::is_arithmetic<ValueType>::value, "Must be arithmetic");
    }

    CachedValue<double> time;
    CachedValue<ValueType> value;
    CachedValue<int> type;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AutomationMarker)
};
