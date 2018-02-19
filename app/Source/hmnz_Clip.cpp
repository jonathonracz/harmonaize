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

Clip::Clip (const ValueTree& v, UndoManager* um, const Track* _track)
    : ValueTreeObject (v, um),
      start (getState(), IDs::ClipProps::Start, getUndoManager(), std::numeric_limits<double>::lowest()),
      length (getState(), IDs::ClipProps::Length, getUndoManager(), 0.0),
      color (getState(), IDs::ClipProps::Color, getUndoManager(), _track->color),
      type (getState(), IDs::TrackProps::Type, getUndoManager(), _track->type),
      track (_track),
      midiMessageSequenceModel (getState().getOrCreateChildWithName (MidiMessageSequenceModel::identifier, nullptr), getUndoManager())
{
}
