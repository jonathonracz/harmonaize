/*
  ==============================================================================

    hmnz_ValueTreeObject.h
    Created: 22 Jan 2018 1:25:56pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_Identifiers.h"

/**
    A run-time class which represents an abstract ValueTree node.
*/
template<const Identifier& identifierType>
class ValueTreeObject : protected ValueTree::Listener
{
public:
    ValueTreeObject (const ValueTree& v, UndoManager* um)
        : state (v), undoManager (um) {}

    virtual ~ValueTreeObject() = default;

    ValueTree& getState()
    {
        return state;
    }

    UndoManager* getUndoManager() const
    {
        return undoManager;
    }

    void beginNewTransaction (const String& actionName = String())
    {
        if (undoManager)
            undoManager->beginNewTransaction (actionName);
    }

    static constexpr const Identifier& identifier = identifierType;

protected:
    void valueTreePropertyChanged (ValueTree&, const Identifier&) override {}
    void valueTreeChildAdded (ValueTree&, ValueTree&) override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

private:
    ValueTree state;
    UndoManager* undoManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ValueTreeObject)
};
