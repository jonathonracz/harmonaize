/*
  ==============================================================================

    hmnz_MasterTrack.cpp
    Created: 30 Jan 2018 6:31:27pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_MasterTrack.h"
#include "hmnz_Edit.h"
#include <ratio>

MasterTrack::MasterTrack (const ValueTree& v, UndoManager* um, Edit* const _edit)
    : ValueTreeObject (v, um),
      tempo (getState().getOrCreateChildWithName (Tempo::identifier, nullptr), getUndoManager()),
      timeSignature (getState().getOrCreateChildWithName (TimeSignature::identifier, nullptr), getUndoManager()),
      keySignature (getState().getOrCreateChildWithName (KeySignature::identifier, nullptr), getUndoManager()),
      edit (_edit)
{
}

MidiMessageSequence MasterTrack::createMetaEventsSequence() const noexcept
{
    // TODO: This is extremely rudimentary and does not support automation
    MidiMessageSequence ret;
    auto microsecondsPerBeat = [](double bpm) -> int {
        return static_cast<int> (60000000.0 / bpm);
    };

    ret.addEvent (MidiMessage::tempoMetaEvent (microsecondsPerBeat (tempo.tempoAtTime (0.0))));
    TimeSignature::Snapshot timeSig = timeSignature.getTimeSignatureAtTime (0.0);
    ret.addEvent (MidiMessage::timeSignatureMetaEvent (timeSig.numerator, timeSig.denominator));
    KeySignature::Snapshot keySig = keySignature.getKeySignatureAtTime (0.0);
    ret.addEvent (MidiMessage::keySignatureMetaEvent (keySig.numSharpsOrFlats, keySig.isMinor));

    return ret;
}
