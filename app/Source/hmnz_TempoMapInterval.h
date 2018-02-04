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
    TempoMapInterval (const ValueTree& v, UndoManager* um, const ValueTree& m1, const ValueTree& m2) noexcept
        : ValueTreeObject (v, um), markerBefore (m1), markerAfter (m2)
    {
        jassert (markerBefore.getType() == IDs::AutomationMarker && markerAfter.getType() == IDs::AutomationMarker);
    }

    void recalculateInterval() noexcept
    {
        double b0 = markerBefore[IDs::AutomationMarkerProps::Beat];
        double b1 = markerAfter[IDs::AutomationMarkerProps::Beat];
        double t0 = markerBefore[IDs::AutomationMarkerProps::Value];
        double t1 = markerAfter[IDs::AutomationMarkerProps::Value];

        std::unique_ptr<TempoFunctions::TempoFunction> tempoFunction;
        int type = markerAfter[IDs::AutomationMarkerProps::Type];
        switch (type)
        {
            case AutomationMarker<double>::Type::linear:
            {
                tempoFunction = std::unique_ptr<TempoFunctions::TempoFunction> (new TempoFunctions::Linear (b0, b1, t0, t1, getPriorIntervalEndTime()));
                break;
            }
            case AutomationMarker<double>::Type::step:
            {
                tempoFunction = std::unique_ptr<TempoFunctions::TempoFunction> (new TempoFunctions::Step (b0, b1, t0, t1, getPriorIntervalEndTime()));
                break;
            }
            default: jassertfalse;
        }

        double endTime = tempoFunction->time (b1);
        double endPeriod = tempoFunction->beatPeriod (b1);

        getState().setProperty (IDs::TempoMapIntervalProps::EndTime, endTime, nullptr);
        getState().setProperty (IDs::TempoMapIntervalProps::EndPeriod, endPeriod, nullptr);
    }

private:
    ValueTree markerBefore;
    ValueTree markerAfter;

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
