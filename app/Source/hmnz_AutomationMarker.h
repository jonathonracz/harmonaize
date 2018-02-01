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
*/
template<typename ValueType>
class AutomationMarker  : public ValueTreeObject<IDs::AutomationMarker>
{
public:
    enum Type : int
    {
        origin,
        linear,
        exponential,
        step
    };

    AutomationMarker (const ValueTree& v, UndoManager* um)
        : ValueTreeObject (v, um),
          beat (getState(), IDs::AutomationMarkerProps::Beat, getUndoManager()),
          value (getState(), IDs::AutomationMarkerProps::Value, getUndoManager()),
          type (getState(), IDs::AutomationMarkerProps::Type, getUndoManager())
    {
        static_assert(std::is_arithmetic<ValueType>::value, "Must be arithmetic");
        jassert (v.getType() == IDs::AutomationMarker);
    }

    static ValueTree createDefaultState()
    {
        ValueTree ret (identifier);
        ret[IDs::AutomationMarkerProps::Beat] = 0.0;
        ret[IDs::AutomationMarkerProps::Value] = 0.0;
        ret[IDs::AutomationMarkerProps::Type] = Type::origin;
    }

    CachedValue<SPSCAtomicWrapper<double>> beat;
    CachedValue<SPSCAtomicWrapper<ValueType>> value;
    CachedValue<SPSCAtomicWrapper<int>> type;
};
