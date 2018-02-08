/*
  ==============================================================================

    hmnz_Track.cpp
    Created: 28 Jan 2018 12:32:09pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_Track.h"
#include "hmnz_Edit.h"

Track::Track (const ValueTree& v, UndoManager* um, Edit* const _edit)
    : ValueTreeObject (v, um),
      edit (_edit),
      name (getState(), IDs::TrackProps::Name, getUndoManager(), "New Track"),
      color (getState(), IDs::TrackProps::Color, getUndoManager(), Colour (Random::getSystemRandom().nextInt(255), Random::getSystemRandom().nextInt(255), Random::getSystemRandom().nextInt(255))),
      type (getState(), IDs::TrackProps::Type, getUndoManager(), IDs::TrackProps::Types::Midi),
      clips (this)
{
    Utility::writeBackDefaultValueIfNotThere (name);
    Utility::writeBackDefaultValueIfNotThere (color);
    Utility::writeBackDefaultValueIfNotThere (type);
}

void Track::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{

}

void Track::releaseResources()
{

}

void Track::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    AudioPlayHead currentPlayhead;
    bool playheadResult = edit->transport->getCurrentPosition (currentPlayhead);
    jassert (playheadResult);
}
