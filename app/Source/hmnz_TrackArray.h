/*
  ==============================================================================

    hmnz_TrackArray.h
    Created: 28 Jan 2018 3:16:29pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_Track.h"
#include "hmnz_ValueTreeObjectArray.h"

class TrackArray    : public ValueTreeObjectArray<Track, CriticalSection>
{
public:
    TrackArray (Edit* const edit);

private:
    Edit* const edit;

    Track* createNewObject (const ValueTree& v, UndoManager* um) override;
};
