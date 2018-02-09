/*
  ==============================================================================

    hmnz_Note.cpp
    Created: 28 Jan 2018 1:44:47pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_Note.h"
#include "hmnz_Clip.h"
#include "hmnz_ValueTreeObject.h"

Note::Note (const ValueTree& v, UndoManager* um, const Clip* _owner)
    : ValueTreeObject (v, um),
      start (getState(), IDs::NoteProps::Start, getUndoManager()),
      length (getState(), IDs::NoteProps::Length, getUndoManager()),
      velocity (getState(), IDs::NoteProps::Velocity, getUndoManager()),
      value (getState(), IDs::NoteProps::Value, getUndoManager()),
      owner (_owner)
{
}
