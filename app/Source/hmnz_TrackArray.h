/*
  ==============================================================================

    hmnz_TrackArray.h
    Created: 28 Jan 2018 3:16:29pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_Track.h"
#include "hmnz_HomogeneousValueTreeObjectArray.h"

class TrackArray    : public HomogeneousValueTreeObjectArray<Track, CriticalSection>
{
public:
    TrackArray (const ValueTree& v, UndoManager* um, Edit* edit);

private:
    Edit* const edit;
    Track* createNewObject (const ValueTree& v, UndoManager* um) override;
};
