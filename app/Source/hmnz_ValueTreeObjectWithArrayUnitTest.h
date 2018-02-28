/*
  ==============================================================================

    hmnz_ValueTreeObjectWithArrayUnitTest.h
    Created: 28 Feb 2018 4:36:36pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObjectArrayUnitTest.h"

class ValueTreeObjectWithArrayUnitTest   : public UnitTest
{
public:
    using TestObject = ValueTreeObjectUnitTest::TestValueTreeObject;

    class ObjectWithArray   : public TestObject
    {
    public:
        ObjectWithArray (const ValueTree& v, UndoManager* um)
            : TestObject (v, um), array (v, um)
        {
            ValueTree internallyAddedState = TestObject::createDefaultState();
            array.addState (internallyAddedState);
        }

        GenericValueTreeObjectArray<TestObject> array;
    };

    ValueTreeObjectWithArrayUnitTest()
        : UnitTest ("ValueTreeObjectWithArray")
    {
    }

    void runTest() override
    {
        beginTest ("Test");

        ValueTree defaultState = TestObject::createDefaultState();
        UndoManager defaultUm;
        ObjectWithArray testObject (defaultState, &defaultUm);
    }
};
