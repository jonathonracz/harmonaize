/*
  ==============================================================================

    hmnz_MasterTrack.cpp
    Created: 30 Jan 2018 6:31:27pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_MasterTrack.h"
#include "hmnz_Application.h"

MasterTrack::MasterTrack (const ValueTree& v, UndoManager* um)
    : ValueTreeObject (v, um),
      tempo (getState().getOrCreateChildWithName (Tempo::identifier, nullptr), getUndoManager()),
      timeSignature (getState().getOrCreateChildWithName (TimeSignature::identifier, nullptr), getUndoManager()),
      keySignature (getState().getOrCreateChildWithName (KeySignature::identifier, nullptr), getUndoManager()),
      genre (getState().getOrCreateChildWithName(IDs::Genre, nullptr), getUndoManager()),
      metronomeEnabled (getState(), IDs::MasterTrackProps::MetronomeEnabled, getUndoManager(), false)
{
    midiBuffer.ensureSize (2048);

    File instrumentsDirectory = SFZInstrumentBank::getInstrumentsDirectory();
    metronome.addVoice (new sfzero::Voice);
    sfzero::Sound* metronomeSound = new sfzero::Sound (instrumentsDirectory.getChildFile ("./Metronome/Metronome.sfz"));
    metronomeSound->loadRegions();
    metronomeSound->loadSamples (&HarmonaizeApplication::getFormatManager());
    metronome.addSound (metronomeSound);
}

void MasterTrack::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    metronome.setCurrentPlaybackSampleRate (sampleRate);
}

void MasterTrack::releaseResources()
{

}

void MasterTrack::getNextAudioBlockWithInputs (AudioBuffer<float>& audioBuffer, MidiBuffer& incomingMidiBuffer, PlaybackEngine& playbackSource)
{
    AudioPlayHead::CurrentPositionInfo currentPosition;
    bool result = playbackSource.getCurrentPosition (currentPosition);
    jassert (result);

    if (!metronomeEnabled.get() || !currentPosition.isRecording)
        return;

    auto beatForSample = [&](int64 sample) -> double {
        return tempo.beat (sample / playbackSource.getActiveSampleRate());
    };

    auto sampleForBeat = [&](double beat) -> int64 {
        return static_cast<int64> (tempo.time (beat) * playbackSource.getActiveSampleRate());
    };

    midiBuffer.clear();

    double beatStart = std::ceil (beatForSample (currentPosition.timeInSamples));
    double beatEnd = std::floor (beatForSample (currentPosition.timeInSamples + audioBuffer.getNumSamples()));
    const int lowClickNote = 60;
    const int highClickNote = 72;
    while (beatStart <= beatEnd)
    {
        int sample = static_cast<int> (sampleForBeat (beatStart));
        midiBuffer.addEvent (MidiMessage::allNotesOff (1), sample);
        midiBuffer.addEvent (MidiMessage::noteOn (1, timeSignature.isFirstBeatInBar (beatStart) ? highClickNote : lowClickNote, 1.0f), sample);
        beatStart += 1.0;
    }

    metronome.renderNextBlock (audioBuffer, midiBuffer, 0, audioBuffer.getNumSamples());
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
    String genreName = Genre::getStringFromIndex (genre.genre);
    ret.addEvent (MidiMessage::textMetaEvent(1, genreName));

    return ret;
}
