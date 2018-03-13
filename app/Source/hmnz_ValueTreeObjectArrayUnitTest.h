/*
  ==============================================================================

    hmnz_ValueTreeObjectArrayUnitTest.h
    Created: 28 Feb 2018 4:25:37pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObjectUnitTest.h"
#include "hmnz_HomogeneousValueTreeObjectArray.h"
#include "hmnz_NonHomogeneousValueTreeObjectArray.h"

class ValueTreeObjectArrayUnitTest   : public UnitTest
{
public:
    using TestObject = ValueTreeObjectUnitTest::TestValueTreeObject<double>;

    ValueTreeObjectArrayUnitTest()
        : UnitTest ("ValueTreeObjectArray")
    {
    }

    void runTest() override
    {
        beginTest ("Test");

        ValueTree defaultState = TestObject::createDefaultState();
        UndoManager defaultUm;

        {
            GenericHomogeneousValueTreeObjectArray<TestObject> testArray (defaultState, &defaultUm);
            testArray.addState (TestObject::createDefaultState());
            defaultState.addChild (TestObject::createDefaultState(), 0, &defaultUm);
            testArray.removeObject (testArray.getFirst());
            defaultState.removeChild (0, &defaultUm);
        }

        {
            GenericNonHomogeneousValueTreeObjectArray<TestObject> testArray (defaultState, &defaultUm);
            testArray.addState (TestObject::createDefaultState());
            defaultState.addChild (TestObject::createDefaultState(), 0, &defaultUm);
            testArray.removeObject (testArray.getFirst());
            defaultState.removeChild (0, &defaultUm);
        }
    }
};
