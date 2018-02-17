/*
  ==============================================================================

    hmnz_Track.cpp
    Created: 28 Jan 2018 12:32:09pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_Track.h"
#include "hmnz_Edit.h"
#include "hmnz_VariantConverters.h"

Track::Track (const ValueTree& v, UndoManager* um, Edit* const _edit)
    : ValueTreeObject (v, um),
      name (getState(), IDs::TrackProps::Name, getUndoManager(), "New Track"),
      color (getState(), IDs::TrackProps::Color, getUndoManager(), Colour (Random::getSystemRandom().nextInt(255), Random::getSystemRandom().nextInt(255), Random::getSystemRandom().nextInt(255))),
      type (getState(), IDs::TrackProps::Type, getUndoManager(), IDs::TrackProps::Types::Midi),
      recordArmed (getState(), IDs::TrackProps::RecordArmed, nullptr, false),
      edit (_edit),
      clipList (getState().getOrCreateChildWithName (IDs::ClipList, nullptr), getUndoManager(), this)
{
    Utility::writeBackDefault (name);
    Utility::writeBackDefault (color);
    Utility::writeBackDefault (type);

    // May not entirely prevent allocation in the audio callback, but it will
    // at least provide a good starting point. 2048 is what's used internally
    // by JUCE.
    midiBuffer.ensureSize (2048);
}

void Track::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    audioBuffer.setSize (2, samplesPerBlockExpected); // TODO: Hardcoded number of channels.
}

void Track::releaseResources()
{

}

void Track::getNextAudioBlockWithInputs (AudioBuffer<float>& audioBuffer,
                                  const MidiBuffer& incomingMidiBuffer,
                                  const AudioPlayHead::CurrentPositionInfo& positionInfo)
{
    auto beatForLocalSample = [&](int64 sample) -> double {
        return edit->masterTrack->tempo->beat ((positionInfo.timeInSamples + sample) / edit->transport->sampleRate.get());
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
