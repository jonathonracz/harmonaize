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
class ValueTreeObject : private ValueTree::Listener
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

    const UndoManager* getUndoManager() const
    {
        return undoManager;
    }

    static constexpr const Identifier& identifier = identifierType;

protected:
    Value getPropertyAsValue (const Identifier& name, var defaultValue = var())
    {
        Value ret = state.getPropertyAsValue (name, undoManager);
        if (ret.getValue().isUndefined() && !defaultValue.isUndefined())
            ret = defaultValue;

        return ret;
    }

    Value getPropertyAsValueChecked (const Identifier& name)
    {
        jassert (state.hasProperty (name));
        Value ret = getPropertyAsValue (name, undoManager);
        return ret;
    }

    virtual void statePropertyChanged(const Identifier& property) {}
    virtual void stateParentChanged() {}

private:
    ValueTree state;
    UndoManager* undoManager;

    void valueTreePropertyChanged (ValueTree& tree, const Identifier& property) override
    {
        if (tree == state)
            statePropertyChanged (property);
    }

    void valueTreeChildAdded (ValueTree&, ValueTree&) override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override
    {
        stateParentChanged();
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ValueTreeObject)
};
