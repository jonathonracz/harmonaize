/*
  ==============================================================================

    hmnz_MasterTrack.cpp
    Created: 30 Jan 2018 6:31:27pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_MasterTrack.h"
#include "hmnz_Edit.h"

MasterTrack::MasterTrack (const ValueTree& v, UndoManager* um, Edit* const _edit)
    : ValueTreeObject (v, um),
      edit (_edit)
{
    tempo = std::unique_ptr<Tempo> (new Tempo (getState().getOrCreateChildWithName (Tempo::identifier, nullptr), getUndoManager()));
    timeSignature = std::unique_ptr<TimeSignature> (new TimeSignature (getState().getOrCreateChildWithName (TimeSignature::identifier, nullptr), getUndoManager()));
    keySignature = std::unique_ptr<KeySignature> (new KeySignature (getState().getOrCreateChildWithName (KeySignature::identifier, nullptr), getUndoManager()));
}
