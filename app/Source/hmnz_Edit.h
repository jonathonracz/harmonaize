/*
  ==============================================================================

    hmnz_Edit.h
    Created: 22 Jan 2018 12:56:24am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObjectArray.h"
#include "hmnz_Track.h"

/**
    Represents an active edit (also known as a project).
*/
class Edit  : public ValueTreeObject<IDs::Edit>
{
public:
    Edit (const ValueTree& v)
        : ValueTreeObject (v, &undoManager), tracks (v, &undoManager)
    {
        ValueTree state = getState();
        stateDebugger.setSource (state);
    }

private:
    UndoManager undoManager;
    ValueTreeObjectArray<Track> tracks;

    jcf::ValueTreeDebugger stateDebugger;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Edit)
};
