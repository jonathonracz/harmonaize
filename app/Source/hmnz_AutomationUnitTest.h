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
        ValueTree defaultAutomation = Automation<double>::createDefaultState();
        Automation<double> automation (defaultAutomation, nullptr);

        ValueTree newMarker1 = AutomationMarker<double>::createDefaultState();
        newMarker1.setProperty (IDs::AutomationMarkerProps::Type, AutomationMarker<double>::Type::linear, nullptr);
        newMarker1.setProperty (IDs::AutomationMarkerProps::Value, 5.0f, nullptr);
        newMarker1.setProperty (IDs::AutomationMarkerProps::Beat, 4.0f, nullptr);

        automation.addMarker (newMarker1);

        ValueTree newMarker2 = AutomationMarker<double>::createDefaultState();
        newMarker2.setProperty (IDs::AutomationMarkerProps::Type, AutomationMarker<double>::Type::step, nullptr);
        newMarker2.setProperty (IDs::AutomationMarkerProps::Value, 4.0f, nullptr);
        newMarker2.setProperty (IDs::AutomationMarkerProps::Beat, 2.0f, nullptr);

        automation.addMarker (newMarker2);

        //automation.
    }
};
