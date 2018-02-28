/*
  ==============================================================================

    hmnz_ValueTreeObjectArrayUnitTest.h
    Created: 28 Feb 2018 4:25:37pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_GenericValueTreeObjectArray.h"
#include "hmnz_ValueTreeObjectUnitTest.h"

class ValueTreeObjectArrayUnitTest   : public UnitTest
{
public:
    using TestObject = ValueTreeObjectUnitTest::TestValueTreeObject;

    ValueTreeObjectArrayUnitTest()
        : UnitTest ("ValueTreeObjectArray")
    {
    }

    void runTest() override
    {
        beginTest ("Test");

        ValueTree defaultState = TestObject::createDefaultState();
        UndoManager defaultUm;
        GenericValueTreeObjectArray<TestObject> testArray (defaultState, &defaultUm);

        testArray.addState (TestObject::createDefaultState());
        defaultState.addChild (TestObject::createDefaultState(), 0, &defaultUm);
        testArray.removeObject (testArray.getFirst());
        defaultState.removeChild (0, &defaultUm);
    }
};
