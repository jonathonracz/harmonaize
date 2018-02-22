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
      edit (_edit)
{
    tempo = std::unique_ptr<Tempo> (new Tempo (getState().getOrCreateChildWithName (Tempo::identifier, nullptr), getUndoManager()));
    timeSignature = std::unique_ptr<TimeSignature> (new TimeSignature (getState().getOrCreateChildWithName (TimeSignature::identifier, nullptr), getUndoManager()));
    keySignature = std::unique_ptr<KeySignature> (new KeySignature (getState().getOrCreateChildWithName (KeySignature::identifier, nullptr), getUndoManager()));
}

MidiMessageSequence MasterTrack::createMetaEventsSequence() const noexcept
{
    // TODO: This is extremely rudimentary and does not support automation
    MidiMessageSequence ret;
    auto microsecondsPerBeat = [](double bpm) -> int {
        return static_cast<int> (1.0f / ((bpm / 60.0) / std::milli::den));
    };

    ret.addEvent (MidiMessage::tempoMetaEvent (microsecondsPerBeat (tempo->tempoAtTime (0.0))));
    ret.addEvent (MidiMessage::timeSignatureMetaEvent (timeSignature->numerator.get(), timeSignature->denominator.get()));
    ret.addEvent (MidiMessage::keySignatureMetaEvent (keySignature->numSharpsOrFlats.get(), keySignature->isMinor.get()));

    return ret;
}
