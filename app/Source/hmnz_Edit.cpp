/*
  ==============================================================================

    hmnz_Edit.cpp
    Created: 28 Jan 2018 12:49:59am
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_Edit.h"
#include "hmnz_Application.h"
#include "hmnz_Transport.h"
#include "hmnz_Utility.h"

Edit::Edit (const ValueTree& v)
    : ValueTreeObject<IDs::Edit> (v, &undoManager),
      tracks (this)
{
    // TODO: Validate the ValueTree data model, display an error if
    // something unexpected occurs, etc...

    masterTrack = std::unique_ptr<MasterTrack> (new MasterTrack (getState().getOrCreateChildWithName (MasterTrack::identifier, nullptr), getUndoManager(), this));
    transport = std::unique_ptr<Transport> (new Transport (getState().getOrCreateChildWithName (Transport::identifier, nullptr), getUndoManager(), this));

    getState().addListener (this);

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
    masterTrack->prepareToPlay (samplesPerBlockExpected, sampleRate);
    for (Track* track : tracks.objects)
    {
        track->prepareToPlay (samplesPerBlockExpected, sampleRate);
    }
}

void Edit::releaseResources()
{
    masterTrack->releaseResources();
    for (Track* track : tracks.objects)
    {
        track->releaseResources();
    }
}

void Edit::getNextAudioBlockWithInputs (AudioBuffer<float>& audioBuffer,
        const MidiBuffer& incomingMidiBuffer,
        const AudioPlayHead::CurrentPositionInfo& positionInfo)
{
    for (Track* track : tracks.objects)
    {
        track->getNextAudioBlockWithInputs (audioBuffer, incomingMidiBuffer, positionInfo);
    }
}
