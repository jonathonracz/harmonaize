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
    AutomationUnitTest()
        : UnitTest ("Automation")
    {
    }

    void runTest() override
    {
        Automation<double> automation;

        ValueTree newMarker1 = AutomationMarker<double>::createDefaultState();
        newMarker.setProperty (IDs::AutomationMarkerProps::Type, AutomationMarker<double>::Type::linear);
        newMarker.setProperty (IDs::AutomationMarkerProps::Value, 5.0f, nullptr);
        newMarker.setProperty (IDs::AutomationMarkerProps::Beat, 4.0f, nullptr);

        automation.addMarker (newMarker1);

        ValueTree newMarker2 = AutomationMarker<double>::createDefaultState();
        newMarker.setProperty (IDs::AutomationMarkerProps::Type, AutomationMarker<double>::Type::exponential);
        newMarker.setProperty (IDs::AutomationMarkerProps::Value, 4.0f, nullptr);
        newMarker.setProperty (IDs::AutomationMarkerProps::Beat, 2.0f, nullptr);

        automation.addMarker (newMarker2);

        //automation.
    }
};

static AutomationUnitTest automationUnitTest;
