/*
  ==============================================================================

    hmnz_Clip.cpp
    Created: 28 Jan 2018 1:45:02pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_Clip.h"
#include "hmnz_Track.h"
#include "hmnz_ValueTreeObject.h"

Clip::Clip (const ValueTree& v, UndoManager* um, const Track* _owner)
    : ValueTreeObject (v, um),
      start (getState(), IDs::ClipProps::Start, getUndoManager(), 0.0),
      length (getState(), IDs::ClipProps::Length, getUndoManager(), 4.0),
      color (getState(), IDs::ClipProps::Color, getUndoManager(), _owner->color),
      type (getState(), IDs::TrackProps::Type, getUndoManager(), _owner->type),
      owner (_owner),
      notes (this)
{
}
