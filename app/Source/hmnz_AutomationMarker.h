/*
  ==============================================================================

    hmnz_AutomationMarker.h
    Created: 31 Jan 2018 5:00:58pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

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
    enum Type : int
    {
        origin,
        linear,
        step
    };

    AutomationMarker (const ValueTree& v, UndoManager* um)
        : ValueTreeObject (v, um),
          // Due to sibling constraints and other factors, undo is handled by the owner Automation object.
          beat (getState(), IDs::AutomationMarkerProps::Beat, nullptr),
          value (getState(), IDs::AutomationMarkerProps::Value, nullptr),
          type (getState(), IDs::AutomationMarkerProps::Type, nullptr)
    {
        static_assert(std::is_arithmetic<ValueType>::value, "Must be arithmetic");
        jassert (v.getType() == IDs::AutomationMarker);
    }

    static ValueTree createDefaultState()
    {
        ValueTree ret (identifier);
        ret.setProperty (IDs::AutomationMarkerProps::Beat, std::numeric_limits<double>::lowest(), nullptr);
        ret.setProperty (IDs::AutomationMarkerProps::Value, ValueType(), nullptr);
        ret.setProperty (IDs::AutomationMarkerProps::Type, Type::origin, nullptr);
        return ret;
    }

    CachedValue<SPSCAtomicWrapper<double>> beat;
    CachedValue<SPSCAtomicWrapper<ValueType>> value;
    CachedValue<SPSCAtomicWrapper<int>> type;
};
