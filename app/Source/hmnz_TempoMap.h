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
    TempoMap (const ValueTree& editState, Automation<double>& automation)
        : automationSource (automation), editRoot (editState)
    {
        automationSource.getState().addListener (this);
        jassert (editRoot.getType() == IDs::Edit);
        editRoot.addListener (this);

        editStartMarker = AutomationMarker<double>::createDefaultState();
        double originRoot = editRoot[IDs::EditProps::OriginBeat];
        editStartMarker.setProperty (IDs::AutomationMarkerProps::Beat, originRoot, nullptr);
        editStartMarker.setProperty (IDs::AutomationMarkerProps::Type, AutomationMarker<double>::Type::linear, nullptr);
        editEndMarker = AutomationMarker<double>::createDefaultState();
        editEndMarker.setProperty (IDs::AutomationMarkerProps::Beat, double (editRoot[IDs::EditProps::EndBeat]), nullptr);
        editEndMarker.setProperty (IDs::AutomationMarkerProps::Type, AutomationMarker<double>::Type::linear, nullptr);

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

    double endTime() const noexcept
    {
        return time (editEndMarker[IDs::AutomationMarkerProps::Beat]);
    }

private:
    ValueTree editStartMarker;
    ValueTree editEndMarker;
    Automation<double>& automationSource;
    ValueTree editRoot;
    std::shared_ptr<OwnedArray<TempoFunctions::TempoFunction>> tempoFunction;

    void recomputeTempoIntervals()
    {
        editStartMarker.setProperty (IDs::AutomationMarkerProps::Value, automationSource.getValueAtBeat (editStartMarker[IDs::AutomationMarkerProps::Beat]), nullptr);
        editEndMarker.setProperty (IDs::AutomationMarkerProps::Value, automationSource.getValueAtBeat (editEndMarker[IDs::AutomationMarkerProps::Beat]), nullptr);
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

    std::pair<int, int> getUsableAutomationMarkerBounds() const noexcept
    {
        int usableMarkerStart = 0;
        for (; usableMarkerStart < automationSource.numTimedMarkers(); ++usableMarkerStart)
        {
            if (automationSource.markers.objects[usableMarkerStart]->beat.get() > double (editStartMarker[IDs::AutomationMarkerProps::Beat]))
                break;
        }

        int usableMarkerEnd = automationSource.numTimedMarkers();
        for (; usableMarkerEnd > usableMarkerStart; --usableMarkerEnd)
        {
            if (automationSource.markers.objects[usableMarkerEnd - 1]->beat.get() <= double (editEndMarker[IDs::AutomationMarkerProps::Beat]))
                break;
        }

        return std::make_pair (usableMarkerStart, usableMarkerEnd);
    }

    int getNumMarkers() const noexcept
    {
        std::pair<int, int> markerBounds = getUsableAutomationMarkerBounds();
        return 2 + markerBounds.second - markerBounds.first;
    }

    ValueTree getMarker (int index) const noexcept
    {
        jassert (index < getNumMarkers());
        if (index == 0)
            return editStartMarker;
        else if (index == getNumMarkers() - 1)
            return editEndMarker;

        std::pair<int, int> markerBounds = getUsableAutomationMarkerBounds();
        return automationSource.markers.objects[markerBounds.first + index - 1]->getState();
    }

    void valueTreeChildAdded (ValueTree& parent, ValueTree& addedChild) override
    {
        if (parent == automationSource.getState())
            recomputeTempoIntervals();
    }

    void valueTreeChildRemoved (ValueTree& parent, ValueTree& removedChild, int indexRemovedFrom) override
    {
        if (parent == automationSource.getState())
            recomputeTempoIntervals();
    }

    void valueTreePropertyChanged (ValueTree& treeChanged, const Identifier& property) override
    {
        if (treeChanged == editRoot)
        {
            bool needsRecompute = false;
            if (property == IDs::EditProps::OriginBeat)
            {
                editStartMarker.setProperty (IDs::AutomationMarkerProps::Beat, editRoot[IDs::EditProps::OriginBeat], nullptr);
                needsRecompute = true;
            }
            else if (property == IDs::EditProps::EndBeat)
            {
                editEndMarker.setProperty (IDs::AutomationMarkerProps::Beat, editRoot[IDs::EditProps::EndBeat], nullptr);
                needsRecompute = true;
            }

            if (needsRecompute)
                recomputeTempoIntervals();
        }
    }

    void valueTreeChildOrderChanged (ValueTree &parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex) override {}
    void valueTreeParentChanged (ValueTree &treeWhoseParentHasChanged) override {}
};
