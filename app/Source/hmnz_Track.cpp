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

void Track::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    AudioPlayHead::CurrentPositionInfo currentPosition;
    bool playheadResult = edit->transport->getCurrentPosition (currentPosition);
    jassert (playheadResult);

    jassert (bufferToFill.numSamples - bufferToFill.startSample <= audioBuffer.getNumSamples());
    jassert (bufferToFill.buffer->getNumChannels() <= audioBuffer.getNumChannels());

    //clipList.getClipsForInterval (, <#double end#>)
}
