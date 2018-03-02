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
    template<class Type>
    class ObjectWithArray   : public ValueTreeObject<IDs::TestWithArray>
    {
    public:
        ObjectWithArray (const ValueTree& v, UndoManager* um)
            : ValueTreeObject (v, um), array (v, um)
        {
            ValueTree internallyAddedState = ValueTreeObjectUnitTest::TestValueTreeObject<Type>::createDefaultState();
            array.addState (internallyAddedState);
        }

    private:
        GenericValueTreeObjectArray<ValueTreeObjectUnitTest::TestValueTreeObject<Type>> array;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ObjectWithArray)
    };

    ValueTreeObjectWithArrayUnitTest()
        : UnitTest ("ValueTreeObjectWithArray")
    {
    }

    void runTest() override
    {
        beginTest ("Test");

        ValueTree defaultState = ObjectWithArray<double>::createDefaultState();
        ObjectWithArray<double> testObject (defaultState, nullptr);
    }
};
