/*
  ==============================================================================

    hmnz_ValueTreeObjectUnitTest.h
    Created: 28 Feb 2018 3:35:48pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"
#include "hmnz_Identifiers.h"

class ValueTreeObjectUnitTest   : public UnitTest
{
public:
    template<class Type>
    class TestValueTreeObject   : public ValueTreeObject<IDs::Test>
    {
    public:
        TestValueTreeObject (const ValueTree& v, UndoManager* um)
            : ValueTreeObject (v, um)
        {
        }

    private:
        JUCE_DECLARE_WEAK_REFERENCEABLE (TestValueTreeObject)
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TestValueTreeObject)
    };

    ValueTreeObjectUnitTest()
        : UnitTest ("ValueTreeObject")
    {
    }

    void runTest() override
    {
        beginTest ("Test");

        ValueTree defaultState = TestValueTreeObject<double>::createDefaultState();
        UndoManager defaultUm;
        TestValueTreeObject<double> object (defaultState, &defaultUm);
    }
};
