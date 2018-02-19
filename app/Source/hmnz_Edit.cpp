/*
  ==============================================================================

    hmnz_Edit.cpp
    Created: 28 Jan 2018 12:49:59am
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_Edit.h"
#include "hmnz_Application.h"

Edit::Edit (const ValueTree& v)
    : ValueTreeObject<IDs::Edit> (v, &undoManager),
      masterTrack (getState().getOrCreateChildWithName (MasterTrack::identifier, nullptr), getUndoManager(), this),
      transport (getState().getOrCreateChildWithName (Transport::identifier, nullptr), getUndoManager(), this),
      tracks (this)
{
    // TODO: Validate the ValueTree data model, display an error if
    // something unexpected occurs, etc...
    getState().addListener (this);
    if (tracks.objects.size() == 0)
        tracks.insertStateAtObjectIndex (Track::createDefaultState(), -1);

    stateDebugger.setSource (getState());
}

Edit::~Edit()
{
}

void Edit::valueTreePropertyChanged (ValueTree& tree, const Identifier& id)
{
}

void Edit::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    masterTrack.prepareToPlay (samplesPerBlockExpected, sampleRate);
    for (Track* track : tracks.objects)
    {
        track->prepareToPlay (samplesPerBlockExpected, sampleRate);
    }
}

void Edit::releaseResources()
{
    masterTrack.releaseResources();
    const TrackArray::ScopedLockType sl (tracks.getLock());
    for (Track* track : tracks.objects)
    {
        track->releaseResources();
    }
}

void Edit::getNextAudioBlockWithInputs (AudioBuffer<float>& audioBuffer,
        const MidiBuffer& incomingMidiBuffer,
        const AudioPlayHead::CurrentPositionInfo& positionInfo)
{
    const TrackArray::ScopedLockType sl (tracks.getLock());
    for (Track* track : tracks.objects)
    {
        track->getNextAudioBlockWithInputs (audioBuffer, incomingMidiBuffer, positionInfo);
    }
}
