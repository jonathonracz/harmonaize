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
class ValueTreeObject : ValueTree::Listener
{
public:
    ValueTreeObject (const ValueTree& v, UndoManager* um)
        : state (v), undoManager (um)
    {
        state.addListener (this);
    }

    virtual ~ValueTreeObject() = default;

    ValueTree getState() const
    {
        return state;
    }

    void setState (const ValueTree& v)
    {
        jassert (v.getType() == identifier);
        state = v;
    }

    UndoManager* getUndoManager() const
    {
        return undoManager;
    }

    void setUndoManager (UndoManager* um)
    {
        undoManager = um;
    }

    static constexpr const Identifier& identifier = identifierType;

protected:
    ValueTree state;
    UndoManager* undoManager;

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override {}
    void valueTreeChildAdded (ValueTree&, ValueTree&) override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ValueTreeObject)
};
