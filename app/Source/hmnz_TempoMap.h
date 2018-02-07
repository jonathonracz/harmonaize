/*
  ==============================================================================

    hmnz_TempoMap.h
    Created: 3 Feb 2018 11:11:01am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_TempoFunction.h"

class TempoMap  : public ValueTree::Listener
{
public:
    TempoMap (Automation<double>& automation)
        : automationSource (automation)
    {
        automationSource.getState().addListener (this);
        recomputeTempoIntervals();
    }

    double beat (double time) const noexcept
    {
        std::shared_ptr<OwnedArray<TempoFunctions::TempoFunction>> currentTempoFunction = tempoFunction;
        for (int i = 0; i < currentTempoFunction->size(); ++i)
        {
            TempoFunctions::TempoFunction* currFunction = currentTempoFunction->getUnchecked (i);
            if (time <= currFunction->time (currFunction->b1))
                return currFunction->beat (time);
        }

        jassertfalse;
        return currentTempoFunction->getLast()->beat (time);
    }

    double time (double beat) const noexcept
    {
        std::shared_ptr<OwnedArray<TempoFunctions::TempoFunction>> currentTempoFunction = tempoFunction;
        for (int i = 0; i < currentTempoFunction->size(); ++i)
        {
            TempoFunctions::TempoFunction* currFunction = currentTempoFunction->getUnchecked (i);
            if (beat <= currFunction->b1)
                return currFunction->time (beat);
        }

        jassertfalse;
        return currentTempoFunction->getLast()->time (beat);
    }

    double tempoAtTime (double time) const noexcept
    {
        return tempoAtBeat (beat (time));
    }

    double tempoAtBeat (double beat) const noexcept
    {
        return automationSource.getValueAtBeat (beat);
    }

private:
    Automation<double>& automationSource;
    std::shared_ptr<OwnedArray<TempoFunctions::TempoFunction>> tempoFunction;

    void recomputeTempoIntervals()
    {
        std::shared_ptr<OwnedArray<TempoFunctions::TempoFunction>> newTempoFunction = std::make_shared<OwnedArray<TempoFunctions::TempoFunction>>();
        for (int i = 0; i < getNumMarkers() - 1; ++i)
        {
            ValueTree startMarker = getMarker (i);
            ValueTree endMarker = getMarker (i + 1);

            double b0 = startMarker[IDs::AutomationMarkerProps::Beat];
            double b1 = endMarker[IDs::AutomationMarkerProps::Beat];
            double t0 = startMarker[IDs::AutomationMarkerProps::Value];
            double t1 = endMarker[IDs::AutomationMarkerProps::Value];
            double timeOffset = (i == 0) ? 0.0 : newTempoFunction->getLast()->time (newTempoFunction->getLast()->b0);
            int functionType = endMarker[IDs::AutomationMarkerProps::Type];
            TempoFunctions::TempoFunction* newFunction = nullptr;

            switch (functionType)
            {
                case AutomationMarker<double>::Type::linear:
                {
                    newFunction = new TempoFunctions::Linear (b0, b1, t0, t1, timeOffset);
                    break;
                }
                case AutomationMarker<double>::Type::step:
                {
                    newFunction = new TempoFunctions::Step (b0, b1, t0, t1, timeOffset);
                    break;
                }
                default: jassertfalse;
            }
            newTempoFunction->add (newFunction);
        }

        tempoFunction = newTempoFunction;
    }

    int getNumMarkers() const noexcept
    {
        int numActiveAutomationMarkers = (double (automationSource.markers.objects.getFirst()->beat.get()) == std::numeric_limits<double>::lowest()) ? 0 : automationSource.markers.objects.size();
        return 2 + numActiveAutomationMarkers;
    }

    ValueTree getMarker (int index) const noexcept
    {
        jassert (index < getNumMarkers());
        if (index == 0 || index == getNumMarkers() - 1)
        {
            ValueTree edgeMarker (IDs::AutomationMarker);
            edgeMarker.setProperty (IDs::AutomationMarkerProps::Type, AutomationMarker<double>::Type::step, nullptr);

            double beat = (index == 0) ? 0.0 : std::numeric_limits<double>::max();
            edgeMarker.setProperty (IDs::AutomationMarkerProps::Beat, beat, nullptr);

            double value = (index == 0) ? automationSource.markers.objects.getFirst()->value.get() : automationSource.markers.objects.getLast()->value.get();
            edgeMarker.setProperty (IDs::AutomationMarkerProps::Value, value, nullptr);

            return edgeMarker;
        }

        return automationSource.markers.objects[index - 1]->getState();
    }

    void valueTreeChildAdded (ValueTree& parent, ValueTree& addedChild) override
    {
        recomputeTempoIntervals();
    }

    void valueTreeChildRemoved (ValueTree& parent, ValueTree& removedChild, int indexRemovedFrom) override
    {
        recomputeTempoIntervals();
    }

    void valueTreePropertyChanged (ValueTree& treeChanged, const Identifier& property) override {}
    void valueTreeChildOrderChanged (ValueTree &parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex) override {}
    void valueTreeParentChanged (ValueTree &treeWhoseParentHasChanged) override {}
};
