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

MidiFile Edit::exportToMidi() const noexcept
{
    MidiFile ret;
    ret.setTicksPerQuarterNote (static_cast<int> (transport.pulsesPerQuarterNote.get()));
    MidiMessageSequence trackEvents = masterTrack.createMetaEventsSequence();
    convertTimestampsFromBeatsToTicks (trackEvents);
    ret.addTrack (trackEvents);
    for (Track* track : tracks.objects)
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
    for (int i = 0; i < midiFile.getNumTracks(); ++i)
    {
        MidiMessageSequence trackSequence;
        midiFile.getTrack (i)->extractMidiChannelMessages (i, trackSequence, false);
        for (MidiMessageSequence::MidiEventHolder* event : trackSequence)
        {
            event->message.setTimeStamp ((event->message.getTimeStamp() / midiFile.getTimeFormat()) + timeOffset);
            int destinationTrack = std::max (0, i + trackOffset);
            while (destinationTrack > tracks.objects.size())
                getState().addChild (Track::createDefaultState(), -1, getUndoManager());

            Track* importTargetTrack = tracks.objects[destinationTrack];
            if (importTargetTrack)
                importTargetTrack->addMidiMessageSequenceAsClip (timeOffset, length, trackSequence);
            else
                jassertfalse;
        }
    }
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

void Edit::convertTimestampsFromBeatsToTicks (MidiMessageSequence& sequence) const noexcept
{
    for (MidiMessageSequence::MidiEventHolder* event : sequence)
    {
        double newTimestamp = event->message.getTimeStamp() * transport.pulsesPerQuarterNote.get();
        event->message.setTimeStamp (newTimestamp);
    }
}
