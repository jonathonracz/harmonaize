/*
  ==============================================================================

    hmnz_Automation.h
    Created: 31 Jan 2018 6:06:45pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_AutomationMarkerArray.h"

/**
    Represents an Automation lane containing a collection of beat mapped
    markers.
*/
template<typename ValueType>
class Automation    : public ValueTreeObject<IDs::Automation>
{
public:
    Automation (const ValueTree& v, UndoManager* um, Type originValue = Type())
        : ValueTreeObject (v, um), markers (v)
    {
        if (getState().getNumChildren() == 0)
        {
            // Create a default origin marker.
            getState().addChild (AutomationMarker<ValueType>::createDefaultState());
        }

        validateMarkers();
    }

    AutomationMarkerArray<ValueType> markers;

    static ValueTree createDefaultState()
    {
        // Children will be lazily created after this barebones state is passed
        // to an Automation object.
        return ValueTree (identifier);
    }

private:
    void valueTreeChildAdded (ValueTree& parent, ValueTree& addedChild) override
    {
        jassert (parent == getState());
        jassert (addedChild.getType() == IDs::AutomationMarker);
        validateMarkers();
    }

    void valueTreeChildRemoved (ValueTree& parent, ValueTree&, int) override
    {
        jassert (parent == getState());
        validateMarkers();
    }

    void valueTreeChildOrderChanged (ValueTree& parent, int, int) override
    {
        jassert (parent == getState());
        validateMarkers();
    }

    bool validateMarkers()
    {
        for (int i = 0; i < getState().getNumChildren(); ++i)
        {
            jassert (child.getType() == IDs::AutomationMarker);
            if (i == 0)
                jassert (child[IDs::AutomationMarkerProps::Type] == AutomationMarker<ValueType>::Type::origin);
            else
                jassert (child[IDs::AutomationMarkerProps::Type] != AutomationMarker<ValueType>::Type::origin);
        }
    }
};
