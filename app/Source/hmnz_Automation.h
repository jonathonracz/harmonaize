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
template<typename ValueType, class Constrainer = DefaultConstrainer<ValueType>>
class Automation    : public ValueTreeObject<IDs::Automation>
{
public:
    Automation (const ValueTree& v, UndoManager* um, ValueType originValue = ValueType())
        : ValueTreeObject (v, um), markers (v, um)
    {
        if (getState().getNumChildren() == 0)
        {
            // Create a default origin marker.
            getState().addChild (AutomationMarker<ValueType>::createDefaultState(), -1, nullptr);
        }

        validateMarkers();
    }

    void addMarker (const AutomationMarker<ValueType>& marker)
    {
        addMarker (marker.getState());
    }

    void addMarker (const ValueTree& v)
    {
    #if JUCE_DEBUG
        markerChangedViaMethods = true;
    #endif

        beginNewTransaction ("Add automation marker");
        double beat = v[IDs::AutomationMarkerProps::Beat];
        auto lessThan = [&](ValueTree first, ValueTree second) -> bool {
            return double(first[IDs::AutomationMarkerProps::Beat]) < double(second[IDs::AutomationMarkerProps::Beat]);
        };
        ValueTree markerBefore = *std::lower_bound (getState().begin(), getState().end(), beat, lessThan);
        ValueTree markerAfter = *std::upper_bound (getState().begin(), getState().end(), beat, lessThan);

        int indexOfBefore = getState().getIndexOfChild (markerBefore);
        int indexOfAfter = getState().getIndexOfChild (markerAfter);

        int insertionIndex;
        if (indexOfBefore < 0)
            insertionIndex = 0;
        else
            insertionIndex = indexOfAfter;


        if (indexOfBefore < 0)
        {
            v.setProperty (IDs::AutomationMarkerProps::Type, AutomationMarker<ValueType>::Type::origin, nullptr);
            if (indexOfAfter >= 0)
            {
                jassert (markerAfter[IDs::AutomationMarkerProps::Type] == AutomationMarker<ValueType>::Type::origin);
                markerAfter.setProperty(IDs::AutomationMarkerProps::Type, AutomationMarker<ValueType>::Type::origin, nullptr);
            }
        }

        getState().addChild (v, insertionIndex, getUndoManager());
        validateMarkers();
    }

    void removeMarker (const AutomationMarker<ValueType>& marker)
    {
        removeMarker (marker.getState());
    }

    void removeMarker (const ValueTree& v)
    {
        int indexOfChild = getState().indexOf (v);
        if (indexOfChild > 0)
            removeMarker (indexOfChild);
    }

    void removeMarker (int index)
    {
    #if JUCE_DEBUG
        markerChangedViaMethods = true;
    #endif

        jassert (index >=0 && index < getState().getNumChildren());
        beginNewTransaction ("Remove automation marker");
        getState().removeChild (index, getUndoManager());
        if (index == 0 && getState().getNumChildren() > 0)
            getState().getChild (index).setProperty (IDs::AutomationMarkerProps::Type, AutomationMarker<ValueType>::Type::origin, getUndoManager());

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
#if JUCE_DEBUG
    bool markerChangedViaMethods = false;
#endif

    void valueTreeChildAdded (ValueTree& parent, ValueTree& addedChild) override
    {
    #if JUCE_DEBUG
        jassert (markerChangedViaMethods);
        markerChangedViaMethods = false;
    #endif
    }

    void valueTreeChildRemoved (ValueTree& parent, ValueTree&, int) override
    {
    #if JUCE_DEBUG
        jassert (markerChangedViaMethods);
        markerChangedViaMethods = false;
    #endif
    }

    void valueTreePropertyChanged (ValueTree& treeChanged, const Identifier& property) override
    {
        int indexOfTreeChanged = getState().indexOf (treeChanged);
        jassert (indexOfTreeChanged >= 0);
        if (property == IDs::AutomationMarkerProps::Type)
        {
            if (indexOfTreeChanged == 0)
                jassert (int (treeChanged[property]) == AutomationMarker<ValueType>::Type::origin);
            else
                jassert (int (treeChanged[property]) != AutomationMarker<ValueType>::Type::origin);

            beginNewTransaction ("Automation marker type change");
            treeChanged.setProperty (property, treeChanged[property], getUndoManager());
        }
        else if (property == IDs::AutomationMarkerProps::Beat)
        {
            ValueTree markerBefore = getState().getChild (indexOfTreeChanged - 1);
            ValueTree markerAfter = getState().getChild (indexOfTreeChanged + 1);

            beginNewTransaction ("Automation marker beat change");
            double newBeat = treeChanged[property];
            if (markerBefore.isValid())
            {
                // Behavior: constrain automation times to be within their
                // siblings.
                double beforeBeat = markerBefore[property];
                treeChanged.setProperty (property, std::max (newBeat, beforeBeat), getUndoManager());
            }
            else if (markerAfter.isValid())
            {
                double afterBeat = markerAfter[property];
                treeChanged.setProperty (property, std::min (newBeat, afterBeat), getUndoManager());
            }
        }
        else if (property == IDs::AutomationMarkerProps::Value)
        {
            beginNewTransaction ("Automation marker value change");
            ValueType constrainedValue = Constrainer::constrain (treeChanged[property]);
            treeChanged.setProperty (property, constrainedValue, getUndoManager());
        }
    }

    void validateMarkers()
    {
        double currentBeat = std::numeric_limits<double>::min();
        for (int i = 0; i < getState().getNumChildren(); ++i)
        {
            ValueTree child = getState().getChild (i);
            jassert (child.getType() == IDs::AutomationMarker);
            jassert (currentBeat < double(child[IDs::AutomationMarkerProps::Beat]));
            currentBeat = child [IDs::AutomationMarkerProps::Beat];
            if (i == 0)
                jassert (int (child[IDs::AutomationMarkerProps::Type]) == AutomationMarker<ValueType>::Type::origin);
            else
                jassert (int (child[IDs::AutomationMarkerProps::Type]) != AutomationMarker<ValueType>::Type::origin);
        }
    }
};
