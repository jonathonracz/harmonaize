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
      clips (this)
{
    Utility::writeBackDefault (name);
    Utility::writeBackDefault (color);
    Utility::writeBackDefault (type);
}

void Track::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    buffer.setSize (2, samplesPerBlockExpected);
}

void Track::releaseResources()
{

}

void Track::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    AudioPlayHead::CurrentPositionInfo currentPosition;
    bool playheadResult = edit->transport->getCurrentPosition (currentPosition);
    jassert (playheadResult);

    jassert (bufferToFill.numSamples - bufferToFill.startSample <= buffer.getNumSamples());
    jassert (bufferToFill.buffer->getNumChannels() <= buffer.getNumChannels());
}
