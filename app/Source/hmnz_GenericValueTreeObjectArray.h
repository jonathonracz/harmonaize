/*
  ==============================================================================

    hmnz_GenericValueTreeObjectArray.h
    Created: 16 Feb 2018 4:47:47pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObjectArray.h"

template<class ObjectType, class CriticalSectionType = DummyCriticalSection>
class GenericValueTreeObjectArray   : public ValueTreeObjectArray<ObjectType, CriticalSectionType>
{
public:
    GenericValueTreeObjectArray (const ValueTree& parentTree, UndoManager* um)
        : ValueTreeObjectArray<ObjectType, CriticalSectionType> (parentTree, um)
    {
        this->addObjects();
    }

private:
    ObjectType* createNewObject (const ValueTree& v, UndoManager* um) override
    {
        return new ObjectType (v, um);
    }
};
