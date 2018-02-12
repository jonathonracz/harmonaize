/*
  ==============================================================================

    hmnz_Clip.h
    Created: 22 Jan 2018 12:57:58am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"
#include "hmnz_NoteArray.h"
#include "hmnz_NoteList.h"

class Track;

class Clip  : public ValueTreeObject<IDs::Clip>
{
public:
    Clip (const ValueTree& v, UndoManager* um, const Track* owner);

    CachedValue<double> start;
    CachedValue<double> length;
    CachedValue<Colour> color;
    CachedValue<Identifier> type;

private:
    const Track* owner;
    NoteList notes;
};
