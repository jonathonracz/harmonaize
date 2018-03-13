/*
  ==============================================================================

    hmnz_MasterTrack.cpp
    Created: 30 Jan 2018 6:31:27pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_MasterTrack.h"

MasterTrack::MasterTrack (const ValueTree& v, UndoManager* um)
    : ValueTreeObject (v, um),
      tempo (getState().getOrCreateChildWithName (Tempo::identifier, nullptr), getUndoManager()),
      timeSignature (getState().getOrCreateChildWithName (TimeSignature::identifier, nullptr), getUndoManager()),
      keySignature (getState().getOrCreateChildWithName (KeySignature::identifier, nullptr), getUndoManager())
{
}

MidiMessageSequence MasterTrack::createMetaEventsSequence() const
{
    // TODO: This is extremely rudimentary and does not support automation
    MidiMessageSequence ret;
    auto microsecondsPerBeat = [](double bpm) -> int {
        return static_cast<int> (60000000.0 / bpm);
    };

    ret.addEvent (MidiMessage::tempoMetaEvent (microsecondsPerBeat (tempo.tempoAtTime (0.0))));
    TimeSignature::Snapshot timeSig = timeSignature.getTimeSignatureAtBeat (0.0);
    ret.addEvent (MidiMessage::timeSignatureMetaEvent (timeSig.numerator, timeSig.denominator));
    KeySignature::Snapshot keySig = keySignature.getKeySignatureAtTime (0.0);
    ret.addEvent (MidiMessage::keySignatureMetaEvent (keySig.numSharpsOrFlats, keySig.isMinor));

    return ret;
}
