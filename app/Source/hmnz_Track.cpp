/*
  ==============================================================================

    hmnz_Track.cpp
    Created: 28 Jan 2018 12:32:09pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_Track.h"
#include "hmnz_Edit.h"
#include "hmnz_Application.h"

Track::Track (const ValueTree& v, UndoManager* um, Edit* const _edit)
    : ValueTreeObject (v, um),
      name (getState(), IDs::TrackProps::Name, getUndoManager(), "New Track"),
      color (getState(), IDs::TrackProps::Color, getUndoManager(), Utility::randomColor()),
      type (getState(), IDs::TrackProps::Type, getUndoManager(), IDs::TrackProps::Types::Midi),
      recordArmed (getState(), IDs::TrackProps::RecordArmed, nullptr, false),
      edit (_edit),
      clipList (getState().getOrCreateChildWithName (IDs::ClipList, nullptr), getUndoManager())
{
    // May not entirely prevent allocation in the audio callback, but it will
    // at least provide a good starting point. 2048 is what's used internally
    // by JUCE.
    midiBuffer.ensureSize (2048);

    for (int i = 0; i < 16; ++i)
        synthesizer.addVoice (new sfzero::Voice);

    File instrumentsDirectory = File::getSpecialLocation (File::SpecialLocationType::userDocumentsDirectory).getChildFile ("./HarmonAIze/Instruments");
    jassert (instrumentsDirectory.isDirectory());
    sfzero::Sound* pianoSound = new sfzero::Sound (instrumentsDirectory.getChildFile ("./UprightPiano/UprightPiano.sfz"));
    pianoSound->loadRegions();
    pianoSound->loadSamples (&HarmonaizeApplication::getFormatManager());
    synthesizer.addSound (pianoSound);
}

void Track::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    audioBuffer.setSize (2, samplesPerBlockExpected); // TODO: Hardcoded number of channels.
    synthesizer.setCurrentPlaybackSampleRate (sampleRate);
}

void Track::releaseResources()
{

}

void Track::getNextAudioBlockWithInputs (AudioBuffer<float>& audioBuffer,
                                  const MidiBuffer& incomingMidiBuffer,
                                  const AudioPlayHead::CurrentPositionInfo& positionInfo)
{
    synthesizer.renderNextBlock (audioBuffer, incomingMidiBuffer, 0, audioBuffer.getNumSamples());

    if (!positionInfo.isPlaying)
        return;

    auto beatForSample = [&](int64 sample) -> double {
        return edit->masterTrack.tempo->beat (sample / edit->transport.getActiveSampleRate());
    };

    /*
    // First write back any incoming MIDI if we're recording.
    if (positionInfo.isRecording && recordArmed.get())
    {
        if (!currentRecordTarget)
        {
            currentRecordTarget = clipList.clipAtTime (bufferStartTime);
            if (!currentRecordTarget)
            {
                // This totally allocates in the audio callback, but it's not
                // the end of the world since we don't really have any other
                // choice here.
                ValueTree newClipState = clipList.getState().addChild (ValueTree (Clip::identifier), -1, getUndoManager());
                currentRecordTarget = clipList.clips.objectWithState (newClipState);
            }
        }

        jassert (currentRecordTarget);
        MidiBuffer::Iterator midiIterator (incomingMidiBuffer);
        MidiMessage iteratorMessage;
        while (midiIterator.)
        currentRecordTarget->noteList.addMessage (<#const juce::MidiMessage &note#>, <#double time#>)
    }
    else
    {
        currentRecordTarget = nullptr;
    }
     */

    // Play back any MIDI that may be present for the current buffer.

}
