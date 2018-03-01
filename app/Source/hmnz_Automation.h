/*
  ==============================================================================

    hmnz_AutomationLane.h
    Created: 31 Jan 2018 6:06:45pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ArrayIterator.h"
#include "hmnz_AutomationMarker.h"
#include "hmnz_GenericValueTreeObjectArray.h"
#include "hmnz_ArrayIterator.h"

/**
    Represents an Automation lane containing a collection of time mapped
    markers.
*/
template<typename ValueType>
class Automation    : public ValueTreeObject<IDs::Automation>
{
public:
    Automation (const ValueTree& v, UndoManager* um, ValueType originValue = ValueType())
        : ValueTreeObject (v, um), markers (v, um)
    {
        //getState().addListener (this);
        if (markers.size() == 0)
        {
            // Create a default origin marker.
            addMarker (std::numeric_limits<double>::lowest(), originValue, AutomationMarker<ValueType>::Type::origin);
        }

        validateMarkers();
    }

    void addMarker (double time, ValueType value, int type) noexcept
    {
    #if JUCE_DEBUG
        markerChangedViaMethods = true;
    #endif

        beginNewTransaction ("Add automation marker");
        auto markerBefore = markerBeforeTime (time);
        auto markerAfter = markerAfterTime (time);

        if (markerBefore && markerBefore->time == std::numeric_limits<double>::lowest())
        {
            markerBefore->time = time;
            markerBefore->value = value;
        }
        else
        {
            int indexOfBefore = markers.indexOf (markerBefore);
            int indexOfAfter = markers.indexOf (markerAfter);

            int insertionIndex;
            if (indexOfBefore < 0)
                insertionIndex = 0;
            else
                insertionIndex = indexOfAfter;

            if (indexOfBefore < 0)
            {
                int originalAutomationType = type;
                type = AutomationMarker<ValueType>::Type::origin;
                if (indexOfAfter >= 0)
                {
                    jassert (markerAfter->type == AutomationMarker<ValueType>::Type::origin);
                    markerAfter->type = originalAutomationType;
                }
            }

            ValueTree newMarkerState = AutomationMarker<ValueType>::createDefaultState();
            newMarkerState.setProperty (IDs::AutomationMarkerProps::Time, time, nullptr);
            newMarkerState.setProperty (IDs::AutomationMarkerProps::Value, value, nullptr);
            newMarkerState.setProperty (IDs::AutomationMarkerProps::Type, type, nullptr);
            markers.insertStateAtObjectIndex (newMarkerState, insertionIndex);
        }

        validateMarkers();
    }

    void removeMarker (AutomationMarker<ValueType>* marker)
    {
        removeMarker (marker->getState());
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

        ValueTree childToRemove = getState().getChild (index);
        jassert (childToRemove.isValid());
        beginNewTransaction ("Remove automation marker");
        if (getState().getNumChildren() == 1)
        {
            childToRemove.setProperty (IDs::AutomationMarkerProps::Time, std::numeric_limits<double>::lowest(), getUndoManager());
        }
        else
        {
            getState().removeChild (index, getUndoManager());
            if (index == 0)
                getState().getChild (index).setProperty (IDs::AutomationMarkerProps::Type, AutomationMarker<ValueType>::Type::origin, getUndoManager());
        }

        validateMarkers();
    }

    jdr::OwnedArrayForwardIteratorConst<ValueType> begin() const noexcept
    {
        return markers.begin();
    }

    jdr::OwnedArrayForwardIteratorConst<ValueType> end() const noexcept
    {
        return markers.end();
    }

    AutomationMarker<ValueType>* const operator[] (int index) const noexcept
    {
        return markers[index];
    }

    AutomationMarker<ValueType>* const getFirst() const noexcept
    {
        return markers.getFirst();
    }

    AutomationMarker<ValueType>* const getLast() const noexcept
    {
        return markers.getLast();
    }

    int size() const noexcept
    {
        return markers.size();
    }

    ValueType getValueAtTime (double time) noexcept
    {
        AutomationMarker<ValueType>* const markerBefore = markerBeforeTime (time);
        AutomationMarker<ValueType>* const markerAfter = markerAfterTime (time);

        jassert (markerBefore || markerAfter);

        if (!markerBefore)
        {
            return markerAfter->value;
        }
        else if (!markerAfter)
        {
            return markerBefore->value;
        }

        int afterType = markerAfter->type;
        double beforeTime = markerBefore->time;
        double afterTime = markerAfter->time;
        double beforeValue = markerBefore->value;
        double afterValue = markerAfter->value;
        ValueType retValue;
        switch (afterType)
        {
            case AutomationMarker<ValueType>::Type::linear:
            {
                if (afterTime == beforeTime) // We'd get division by zero
                {
                    retValue = afterValue;
                }
                else
                {
                    double timeDelta = (time - beforeTime) / (afterTime - beforeTime);
                    double retValueDouble = beforeValue + ((afterValue - beforeValue) * timeDelta);
                    if (std::is_integral<ValueType>::value)
                        retValueDouble = std::round (retValueDouble);

                    retValue = static_cast<ValueType>(retValueDouble);
                }
                break;
            }
            case AutomationMarker<ValueType>::Type::step:
            {
                if (time < afterTime)
                    retValue = beforeValue;
                else
                    retValue = afterValue;
                break;
            }
            default: jassertfalse;
        }

        return retValue;
    }

    int numTimedMarkers() const noexcept
    {
        if (markers.size() == 1 && double (markers.getFirst()->time.get()) == std::numeric_limits<double>::lowest())
            return 0;

        return markers.size();
    }

private:
    GenericValueTreeObjectArray<AutomationMarker<ValueType>> markers;
#if JUCE_DEBUG
    bool markerChangedViaMethods = false;
#endif

    AutomationMarker<ValueType>* const markerBeforeTime (double time)
    {
        auto markerAfter = markerAfterTime (time);
        if (!markerAfter)
        {
            int numObjects = markers.size();
            if (numObjects > 0)
                return markers.getLast();

            return nullptr;
        }

        int indexOfMarkerAfter = markers.indexOf (markerAfter);
        jassert (indexOfMarkerAfter >= 0);
        if (indexOfMarkerAfter == 0)
            return nullptr;

        return markers[indexOfMarkerAfter - 1];
    }

    AutomationMarker<ValueType>* const markerAfterTime (double time)
    {
        auto lessThan = [](AutomationMarker<ValueType>* const first, double second) -> bool {
            return first->time < second;
        };
        auto begin = markers.begin();
        auto end = markers.end();
        auto result = std::lower_bound (begin, end, time, lessThan);
        return (result != end) ? *result : nullptr;
    }

    void valueTreeChildAdded (ValueTree& parent, ValueTree& addedChild) override
    {
    #if JUCE_DEBUG
        jassert (addedChild.getType() == AutomationMarker<ValueType>::identifier);
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
            beginNewTransaction ("Automation marker type change");
            treeChanged.setProperty (property, treeChanged[property], getUndoManager());
        }
        else if (property == IDs::AutomationMarkerProps::Time)
        {
            ValueTree markerBefore = getState().getChild (indexOfTreeChanged - 1);
            ValueTree markerAfter = getState().getChild (indexOfTreeChanged + 1);

            beginNewTransaction ("Automation marker time change");
            double newTime = treeChanged[property];
            if (markerBefore.isValid())
            {
                // Behavior: constrain automation times to be within their
                // siblings.
                double beforeTime = markerBefore[property];
                treeChanged.setProperty (property, std::max (newTime, beforeTime), getUndoManager());
            }
            else if (markerAfter.isValid())
            {
                double afterTime = markerAfter[property];
                treeChanged.setProperty (property, std::min (newTime, afterTime), getUndoManager());
            }
        }
        else if (property == IDs::AutomationMarkerProps::Value)
        {
            beginNewTransaction ("Automation marker value change");
            treeChanged.setProperty (property, treeChanged[property], getUndoManager());
        }
    }

    void validateMarkers()
    {
        double currentTime = std::numeric_limits<double>::lowest();
        jassert (getState().getNumChildren() > 0);
        for (int i = 0; i < getState().getNumChildren(); ++i)
        {
            ValueTree child = getState().getChild (i);
            jassert (child.getType() == IDs::AutomationMarker);
            jassert (currentTime <= double(child[IDs::AutomationMarkerProps::Time]));
            currentTime = child [IDs::AutomationMarkerProps::Time];
            if (i == 0)
                jassert (int (child[IDs::AutomationMarkerProps::Type]) == AutomationMarker<ValueType>::Type::origin);
            else
                jassert (int (child[IDs::AutomationMarkerProps::Type]) != AutomationMarker<ValueType>::Type::origin);
        }
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Automation)
};
