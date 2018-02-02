/*
  ==============================================================================

    hmnz_AutomationUnitTest.h
    Created: 1 Feb 2018 1:59:56pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_Automation.h"

class AutomationUnitTest    : public UnitTest
{
public:
    AutomationUnitTest()
        : UnitTest ("Automation")
    {
    }

    void runTest() override
    {
        beginTest ("Main automation test");

        ValueTree defaultAutomation = Automation<double>::createDefaultState();
        Automation<double> automation (defaultAutomation, nullptr);

        ValueTree newMarker = AutomationMarker<double>::createDefaultState();
        newMarker.setProperty (IDs::AutomationMarkerProps::Type, AutomationMarker<double>::Type::linear, nullptr);
        newMarker.setProperty (IDs::AutomationMarkerProps::Value, 5.0, nullptr);
        newMarker.setProperty (IDs::AutomationMarkerProps::Beat, 4.0, nullptr);
        automation.addMarker (newMarker);

        expect (automation.getValueAtBeat (2.0) == 5.0);
        expect (automation.getValueAtBeat (6.0) == 5.0);

        newMarker = AutomationMarker<double>::createDefaultState();
        newMarker.setProperty (IDs::AutomationMarkerProps::Type, AutomationMarker<double>::Type::step, nullptr);
        newMarker.setProperty (IDs::AutomationMarkerProps::Value, 4.0, nullptr);
        newMarker.setProperty (IDs::AutomationMarkerProps::Beat, 2.0, nullptr);
        automation.addMarker (newMarker);

        expect (automation.getValueAtBeat (1.0) == 4.0);
        expect (automation.getValueAtBeat (3.0) == 4.0);
        expect (automation.getValueAtBeat (4.0) == 5.0);
        expect (automation.getValueAtBeat (5.0) == 5.0);

        newMarker = AutomationMarker<double>::createDefaultState();
        newMarker.setProperty (IDs::AutomationMarkerProps::Type, AutomationMarker<double>::Type::linear, nullptr);
        newMarker.setProperty (IDs::AutomationMarkerProps::Value, 3.0, nullptr);
        newMarker.setProperty (IDs::AutomationMarkerProps::Beat, 3.0, nullptr);
        automation.addMarker (newMarker);

        expect (automation.getValueAtBeat (1.0) == 4.0);
        expect (automation.getValueAtBeat (2.0) == 4.0);
        expect (automation.getValueAtBeat (2.5) == 3.5);
        expect (automation.getValueAtBeat (3.0) == 3.0);
        expect (automation.getValueAtBeat (3.5) == 3.0);
        expect (automation.getValueAtBeat (4.0) == 5.0);
        expect (automation.getValueAtBeat (6.0) == 5.0);

        automation.removeMarker (0);

        expect (automation.getValueAtBeat (2.0) == 3.0);
        expect (automation.getValueAtBeat (3.0) == 3.0);
        expect (automation.getValueAtBeat (3.5) == 3.0);
        expect (automation.getValueAtBeat (4.0) == 5.0);
        expect (automation.getValueAtBeat (6.0) == 5.0);

        automation.removeMarker (1);

        expect (automation.getValueAtBeat (2.0) == 3.0);
        expect (automation.getValueAtBeat (3.0) == 3.0);
        expect (automation.getValueAtBeat (4.0) == 3.0);

        automation.removeMarker (0);

        expect (automation.getValueAtBeat (2.0) == 3.0);
        expect (automation.getValueAtBeat (3.0) == 3.0);
        expect (automation.getValueAtBeat (4.0) == 3.0);
    }
};
