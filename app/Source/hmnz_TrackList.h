/*
  ==============================================================================

    hmnz_TrackList.h
    Created: 4 Mar 2018 10:58:34pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"
#include "hmnz_TrackArray.h"

class Edit;

class TrackList : public ValueTreeObject<IDs::TrackList>
{
public:
    TrackList (const ValueTree& v, UndoManager* um, Edit* _edit)
        : ValueTreeObject (v, um), tracks (getState(), getUndoManager(), _edit),
          edit (_edit)
    {
    }

    TrackArray tracks;

private:
    Edit* const edit;
};
