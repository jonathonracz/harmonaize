/*
  ==============================================================================

    hmnz_Automation.h
    Created: 31 Jan 2018 6:06:45pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_AutomationMarkerArray.h"
#include "hmnz_ValueTreeIterator.h"

/**
    Represents an Automation lane containing a collection of beat mapped
    markers.
*/
template<typename ValueType>
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
            getState().getChild (0).setProperty (IDs::AutomationMarkerProps::Value, originValue, nullptr);
        }

        validateMarkers();
    }

    void addMarker (const AutomationMarker<ValueType>& marker)
    {
        addMarker (marker.getState());
    }

    void addMarker (ValueTree v)
    {
    #if JUCE_DEBUG
        markerChangedViaMethods = true;
    #endif

        beginNewTransaction ("Add automation marker");
        double beat = v[IDs::AutomationMarkerProps::Beat];
        ValueTree markerBefore = markerBeforeBeat (beat);
        ValueTree markerAfter = markerAfterBeat (beat);

        if (double(markerBefore[IDs::AutomationMarkerProps::Beat]) == std::numeric_limits<double>::lowest())
        {
            markerBefore.setProperty (IDs::AutomationMarkerProps::Beat, beat, getUndoManager());
            markerBefore.setProperty (IDs::AutomationMarkerProps::Value, v[IDs::AutomationMarkerProps::Value], getUndoManager());
        }
        else
        {
            int indexOfBefore = getState().indexOf (markerBefore);
            int indexOfAfter = getState().indexOf (markerAfter);

            int insertionIndex;
            if (indexOfBefore < 0)
                insertionIndex = 0;
            else
                insertionIndex = indexOfAfter;

            if (indexOfBefore < 0)
            {
                int originalAutomationType = v[IDs::AutomationMarkerProps::Type];
                v.setProperty (IDs::AutomationMarkerProps::Type, AutomationMarker<ValueType>::Type::origin, nullptr);
                if (indexOfAfter >= 0)
                {
                    jassert (int (markerAfter[IDs::AutomationMarkerProps::Type]) == AutomationMarker<ValueType>::Type::origin);
                    markerAfter.setProperty(IDs::AutomationMarkerProps::Type, originalAutomationType, nullptr);
                }
            }

            getState().addChild (v, insertionIndex, getUndoManager());
        }

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

        ValueTree childToRemove = getState().getChild (index);
        jassert (childToRemove.isValid());
        beginNewTransaction ("Remove automation marker");
        if (getState().getNumChildren() == 1)
        {
            childToRemove.setProperty (IDs::AutomationMarkerProps::Beat, std::numeric_limits<double>::lowest(), getUndoManager());
        }
        else
        {
            getState().removeChild (index, getUndoManager());
            if (index == 0)
                getState().getChild (index).setProperty (IDs::AutomationMarkerProps::Type, AutomationMarker<ValueType>::Type::origin, getUndoManager());
        }

        validateMarkers();
    }

    ValueType getValueAtBeat (double beat)
    {
        ValueTree markerBefore = markerBeforeBeat (beat);
        ValueTree markerAfter = markerAfterBeat (beat);

        jassert (markerBefore.isValid() || markerAfter.isValid());

        if (!markerBefore.isValid())
        {
            return markerAfter[IDs::AutomationMarkerProps::Value];
        }
        else if (!markerAfter.isValid())
        {
            return markerBefore[IDs::AutomationMarkerProps::Value];
        }

        int afterType = markerAfter[IDs::AutomationMarkerProps::Type];
        double beforeBeat = markerBefore[IDs::AutomationMarkerProps::Beat];
        double afterBeat = markerAfter[IDs::AutomationMarkerProps::Beat];
        double beforeValue = markerBefore[IDs::AutomationMarkerProps::Value];
        double afterValue = markerAfter[IDs::AutomationMarkerProps::Value];
        ValueType retValue;
        switch (afterType)
        {
            case AutomationMarker<ValueType>::Type::linear:
            {
                if (afterBeat == beforeBeat) // We'd get division by zero
                {
                    retValue = afterValue;
                }
                else
                {
                    double beatDelta = (beat - beforeBeat) / (afterBeat - beforeBeat);
                    double retValueDouble = beforeValue + ((afterValue - beforeValue) * beatDelta);
                    if (std::is_integral<ValueType>::value)
                        retValueDouble = std::round (retValueDouble);

                    retValue = static_cast<ValueType>(retValueDouble);
                }
                break;
            }
            case AutomationMarker<ValueType>::Type::step:
            {
                if (beat < afterBeat)
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
        if (markers.objects.size() == 1 && double (markers.objects[0]->beat.get()) == std::numeric_limits<double>::lowest())
            return 0;

        return markers.objects.size();
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

    ValueTree markerBeforeBeat (double beat)
    {
        ValueTree markerAfter = markerAfterBeat (beat);
        if (!markerAfter.isValid())
        {
            int numChildren = getState().getNumChildren();
            if (numChildren > 0)
                return getState().getChild (numChildren - 1);

            return ValueTree();
        }

        int indexOfMarkerAfter = getState().indexOf (markerAfter);
        jassert (indexOfMarkerAfter >= 0);
        if (indexOfMarkerAfter == 0)
            return ValueTree();

        return getState().getChild (indexOfMarkerAfter - 1);
    }

    ValueTree markerAfterBeat (double beat)
    {
        auto lessThan = [](ValueTree first, double second) -> bool {
            return double(first[IDs::AutomationMarkerProps::Beat]) < second;
        };
        jdr::ValueTreeForwardIterator begin = jdr::ValueTreeForwardIterator::begin (getState());
        jdr::ValueTreeForwardIterator end = jdr::ValueTreeForwardIterator::end (getState());
        jdr::ValueTreeForwardIterator result = std::lower_bound (begin, end, beat, lessThan);
        return (result != end) ? *result : ValueTree();
    }

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
            treeChanged.setProperty (property, treeChanged[property], getUndoManager());
        }
    }

    void validateMarkers()
    {
        double currentBeat = std::numeric_limits<double>::lowest();
        jassert (getState().getNumChildren() > 0);
        for (int i = 0; i < getState().getNumChildren(); ++i)
        {
            ValueTree child = getState().getChild (i);
            jassert (child.getType() == IDs::AutomationMarker);
            jassert (currentBeat <= double(child[IDs::AutomationMarkerProps::Beat]));
            currentBeat = child [IDs::AutomationMarkerProps::Beat];
            if (i == 0)
                jassert (int (child[IDs::AutomationMarkerProps::Type]) == AutomationMarker<ValueType>::Type::origin);
            else
                jassert (int (child[IDs::AutomationMarkerProps::Type]) != AutomationMarker<ValueType>::Type::origin);
        }
    }
};
