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
        beginTest ("Test");

        ValueTree defaultAutomation = Automation<double>::createDefaultState();
        Automation<double>* fuck = new Automation<double> (defaultAutomation, nullptr);
        delete fuck;

        Automation<double>* automation = new Automation<double> (defaultAutomation, nullptr);

        automation->addMarker (4.0, 5.0, AutomationMarker<double>::Type::linear);

        expect (automation->getValueAtTime (2.0) == 5.0);
        expect (automation->getValueAtTime (6.0) == 5.0);

        automation->addMarker (2.0, 4.0, AutomationMarker<double>::Type::step);

        expect (automation->getValueAtTime (1.0) == 4.0);
        expect (automation->getValueAtTime (3.0) == 4.0);
        expect (automation->getValueAtTime (4.0) == 5.0);
        expect (automation->getValueAtTime (5.0) == 5.0);

        automation->addMarker (3.0, 3.0, AutomationMarker<double>::Type::linear);

        expect (automation->getValueAtTime (1.0) == 4.0);
        expect (automation->getValueAtTime (2.0) == 4.0);
        expect (automation->getValueAtTime (2.5) == 3.5);
        expect (automation->getValueAtTime (3.0) == 3.0);
        expect (automation->getValueAtTime (3.5) == 3.0);
        expect (automation->getValueAtTime (4.0) == 5.0);
        expect (automation->getValueAtTime (6.0) == 5.0);

        automation->removeMarker (0);

        expect (automation->getValueAtTime (2.0) == 3.0);
        expect (automation->getValueAtTime (3.0) == 3.0);
        expect (automation->getValueAtTime (3.5) == 3.0);
        expect (automation->getValueAtTime (4.0) == 5.0);
        expect (automation->getValueAtTime (6.0) == 5.0);

        automation->removeMarker (1);

        expect (automation->getValueAtTime (2.0) == 3.0);
        expect (automation->getValueAtTime (3.0) == 3.0);
        expect (automation->getValueAtTime (4.0) == 3.0);

        automation->removeMarker (0);

        expect (automation->getValueAtTime (2.0) == 3.0);
        expect (automation->getValueAtTime (3.0) == 3.0);
        expect (automation->getValueAtTime (4.0) == 3.0);

        delete automation;
        DBG ("BLAH");
    }
};
