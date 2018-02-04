/*
  ==============================================================================

    hmnz_TempoMapInterval.h
    Created: 3 Feb 2018 11:14:39am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_AutomationMarker.h"
#include "hmnz_TempoFunction.h"

class TempoMapInterval  : public ValueTreeObject<IDs::TempoMapInterval>
{
public:
    TempoMapInterval (const ValueTree& v, UndoManager* um) noexcept
        : ValueTreeObject (v, um) {}

    void setMarkers (const ValueTree& _markerBefore, const ValueTree& _markerAfter)
    {
        jassert (_markerBefore.getType() == IDs::AutomationMarker && _markerAfter.getType() == IDs::AutomationMarker);
        markerBefore = _markerBefore;
        markerBefore.addListener (this);
        markerAfter = _markerAfter;
        markerAfter.addListener (this);
        recalculateInterval();
    }

    bool containsBeat (double beat) noexcept
    {
        return double(markerBefore[IDs::AutomationMarkerProps::Beat]) < beat && double(markerAfter[IDs::AutomationMarkerProps::Beat]) >= beat;
    }

    bool containsTime (double time) noexcept
    {
        return getPriorIntervalEndTime() < time && double(getState()[IDs::TempoMapIntervalProps::EndTime]) >= time;
    }

    double beat (double time) noexcept
    {
        if (! containsTime (time))
        {
            jassertfalse;
            return std::numeric_limits<double>::min();
        }

        return tempoFunction->beat (time);
    }

    double time (double beat) noexcept
    {
        if (! containsBeat (beat))
        {
            jassertfalse;
            return std::numeric_limits<double>::min();
        }

        return tempoFunction->time (beat);
    }

private:
    ValueTree markerBefore;
    ValueTree markerAfter;
    std::shared_ptr<TempoFunctions::TempoFunction> tempoFunction;

    void recalculateInterval() noexcept
    {
        double b0 = markerBefore[IDs::AutomationMarkerProps::Beat];
        double b1 = markerAfter[IDs::AutomationMarkerProps::Beat];
        double t0 = markerBefore[IDs::AutomationMarkerProps::Value];
        double t1 = markerAfter[IDs::AutomationMarkerProps::Value];
        int type = markerAfter[IDs::AutomationMarkerProps::Type];

        switch (type)
        {
            case AutomationMarker<double>::Type::linear:
            {
                tempoFunction = std::make_shared<TempoFunctions::Linear>(b0, b1, t0, t1, getPriorIntervalEndTime());
            }
            case AutomationMarker<double>::Type::step:
            {
                tempoFunction = std::make_shared<TempoFunctions::Step>(b0, b1, t0, t1, getPriorIntervalEndTime());
            }
            default: jassertfalse;
        }

        double endTime = tempoFunction->time (b1);
        double endPeriod = tempoFunction->beatPeriod (b1);

        getState().setProperty (IDs::TempoMapIntervalProps::EndTime, endTime, nullptr);
        getState().setProperty (IDs::TempoMapIntervalProps::EndPeriod, endPeriod, nullptr);
    }

    void valueTreePropertyChanged (ValueTree& treeChanged, const Identifier& property) noexcept override
    {
        if (treeChanged == markerBefore || treeChanged == markerAfter)
            recalculateInterval();
    }

    double getPriorIntervalEndTime() noexcept
    {
        // Assumes the parent has no children that aren't of this type, which
        // by design it shouldn't.
        jassert (getState().getParent().isValid());
        ValueTree priorInterval = getState().getSibling (-1);
        return priorInterval.isValid() ? double(priorInterval[IDs::TempoMapIntervalProps::EndTime]) : 0.0;
    }
};
