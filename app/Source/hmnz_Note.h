/*
  ==============================================================================

    hmnz_Note.h
    Created: 23 Jan 2018 1:41:45am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"

class Note  : public ValueTreeObject<IDs::Note>
{
public:
    Note (const ValueTree& v, UndoManager* um)
        : ValueTreeObject (v, um),
          start (getState(), IDs::NoteProps::Start, getUndoManager()),
          length (getState(), IDs::NoteProps::Length, getUndoManager()),
          velocity (getState(), IDs::NoteProps::Velocity, getUndoManager()),
          value (getState(), IDs::NoteProps::Value, getUndoManager())
    {
    }

    MidiMessage getNoteOn() const noexcept
    {
        return MidiMessage::noteOn (0, value, static_cast<uint8>(velocity));
    }

    MidiMessage getNoteOff() const noexcept
    {
        return MidiMessage::noteOff (0, value, static_cast<uint8>(velocity));
    }

    double getEnd() const noexcept
    {
        return start + length;
    }

    CachedValue<double> start;
    CachedValue<double> length;
    CachedValue<int> velocity;
    CachedValue<int> value;
};
