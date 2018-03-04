/*
  ==============================================================================

    hmnz_Edit.cpp
    Created: 28 Jan 2018 12:49:59am
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_Edit.h"
#include "hmnz_Application.h"

Edit::Edit (const ValueTree& v, bool useUndoManager)
    : ValueTreeObject<IDs::Edit> (v, useUndoManager ? new UndoManager : nullptr),
      masterTrack (getState().getOrCreateChildWithName (MasterTrack::identifier, nullptr), getUndoManager(), this),
      transport (getState().getOrCreateChildWithName (Transport::identifier, nullptr)),
      tracks (this),
      undoManager (getUndoManager())
{
    // TODO: Validate the ValueTree data model, display an error if
    // something unexpected occurs, etc...
    getState().addListener (this);
    if (tracks.size() == 0)
        tracks.insertStateAtObjectIndex (Track::createDefaultState(), -1);

    stateDebugger.setSource (getState());
}

Edit::~Edit()
{
}

MidiFile Edit::exportToMidi() const noexcept
{
    MidiFile ret;
    ret.setTicksPerQuarterNote (static_cast<int> (Transport::pulsesPerQuarterNote));
    MidiMessageSequence trackEvents = masterTrack.createMetaEventsSequence();
    convertTimestampsFromBeatsToTicks (trackEvents);
    ret.addTrack (trackEvents);
    for (Track* track : tracks)
    {
        trackEvents = track->getMidiMessageSequence();
        convertTimestampsFromBeatsToTicks (trackEvents);
        ret.addTrack (trackEvents);
    }

    return ret;
}

void Edit::importFromMidi (const MidiFile& midiFile, int trackOffset, double timeOffset) noexcept
{
    jassert (midiFile.getTimeFormat() > 0); // Only support PPQ.

    double length = midiFile.getLastTimestamp() / midiFile.getTimeFormat();
    while (midiFile.getNumTracks() + trackOffset > tracks.size())
        getState().addChild (Track::createDefaultState(), -1, getUndoManager());

    for (int i = 0; i < midiFile.getNumTracks(); ++i)
    {
        MidiMessageSequence trackSequence (*midiFile.getTrack (i));
        for (MidiMessageSequence::MidiEventHolder* event : trackSequence)
        {
            event->message.setTimeStamp ((event->message.getTimeStamp() / midiFile.getTimeFormat()) + timeOffset);
        }

        int destinationTrack = std::max (0, i + trackOffset);
        Track* importTargetTrack = tracks[destinationTrack];
        if (importTargetTrack)
            importTargetTrack->addMidiMessageSequenceAsClip (timeOffset, length, trackSequence);
        else
            jassertfalse;
    }
}

void Edit::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    keyboardState.reset();
    masterTrack.prepareToPlay (samplesPerBlockExpected, sampleRate);
    for (Track* track : tracks)
    {
        track->prepareToPlay (samplesPerBlockExpected, sampleRate);
    }
}

void Edit::releaseResources()
{
    keyboardState.reset();
    masterTrack.releaseResources();
    const TrackArray::ScopedLockType sl (tracks.getLock());
    for (Track* track : tracks)
    {
        track->releaseResources();
    }
}

void Edit::getNextAudioBlockWithInputs (AudioBuffer<float>& audioBuffer,
        MidiBuffer& incomingMidiBuffer,
        PlaybackEngine& playbackSource)
{
    keyboardState.processNextMidiBuffer (incomingMidiBuffer, 0, audioBuffer.getNumSamples(), true);
    const TrackArray::ScopedLockType sl (tracks.getLock());
    for (Track* track : tracks)
    {
        track->getNextAudioBlockWithInputs (audioBuffer, incomingMidiBuffer, playbackSource);
    }
}

void Edit::convertTimestampsFromBeatsToTicks (MidiMessageSequence& sequence) const noexcept
{
    for (MidiMessageSequence::MidiEventHolder* event : sequence)
    {
        double newTimestamp = event->message.getTimeStamp() * Transport::pulsesPerQuarterNote;
        event->message.setTimeStamp (newTimestamp);
    }
}
