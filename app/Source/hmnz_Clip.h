/*
  ==============================================================================

    hmnz_Clip.h
    Created: 22 Jan 2018 12:57:58am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"
#include "hmnz_MidiMessageModel.h"
#include "hmnz_MidiMessageSequenceModel.h"

class Clip  : public ValueTreeObject<IDs::Clip>
{
public:
    Clip (const ValueTree& v, UndoManager* um)
        : ValueTreeObject (v, um),
          start (getState(), IDs::ClipProps::Start, getUndoManager(), std::numeric_limits<double>::lowest()),
          length (getState(), IDs::ClipProps::Length, getUndoManager(), 0.0),
          color (getState(), IDs::ClipProps::Color, getUndoManager(), Colours::green),
          type (getState(), IDs::TrackProps::Type, getUndoManager(), IDs::ClipProps::Types::Midi),
          midiMessageSequenceModel (getState().getOrCreateChildWithName (MidiMessageSequenceModel::identifier, nullptr), getUndoManager())
    {
    }



    CachedValue<double> start;
    CachedValue<double> length;
    CachedValue<Colour> color;
    CachedValue<Identifier> type;

private:
    MidiMessageSequenceModel midiMessageSequenceModel;
};
