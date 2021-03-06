/*
  ==============================================================================

    hmnz_ClipArray.h
    Created: 11 Mar 2018 12:53:26pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_Clip.h"
#include "hmnz_HomogeneousValueTreeObjectArray.h"

class ClipArray    : public HomogeneousValueTreeObjectArray<Clip>
{
public:
    ClipArray (const ValueTree& v, UndoManager* um, Track& track);

private:
    Track& track;
    Clip* createNewObject (const ValueTree& v, UndoManager* um) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ClipArray)
};
