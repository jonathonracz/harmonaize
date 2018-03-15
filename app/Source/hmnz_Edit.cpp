/*
  ==============================================================================

    hmnz_Edit.cpp
    Created: 28 Jan 2018 12:49:59am
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_Edit.h"
#include "hmnz_Application.h"

Edit::Edit (const ValueTree& v, UndoManager* um)
    : ValueTreeObject<IDs::Edit> (v, um),
      masterTrack (getState().getOrCreateChildWithName (MasterTrack::identifier, nullptr), getUndoManager()),
      transport (getState().getOrCreateChildWithName (Transport::identifier, nullptr)),
      arrangementViewModel (getState().getOrCreateChildWithName (ArrangementViewModel::identifier, nullptr)),
      trackList (getState().getOrCreateChildWithName (TrackList::identifier, nullptr), getUndoManager(), *this)
{
    // TODO: Validate the ValueTree data model, display an error if
    // something unexpected occurs, etc...
    if (trackList.tracks.size() == 0)
        trackList.tracks.insertStateAtObjectIndex (Track::createDefaultState(), -1);
}

Edit::~Edit()
{
}

MidiFile Edit::exportToMidi() const
{
    MidiFile ret;
    ret.setTicksPerQuarterNote (static_cast<int> (Transport::pulsesPerQuarterNote));
    MidiMessageSequence trackEvents = masterTrack.createMetaEventsSequence();
    convertTimestampsFromBeatsToTicks (trackEvents);
    ret.addTrack (trackEvents);
    for (Track* track : trackList.tracks)
    {
        trackEvents = track->getMidiMessageSequence();
        convertTimestampsFromBeatsToTicks (trackEvents);
        ret.addTrack (trackEvents);
    }

    return ret;
}

void Edit::importFromMidi (const MidiFile& midiFile, int trackOffset, double timeOffset)
{
    jassert (midiFile.getTimeFormat() > 0); // Only support PPQ.

    double length = midiFile.getLastTimestamp() / midiFile.getTimeFormat();
    while (midiFile.getNumTracks() + trackOffset > trackList.tracks.size())
        trackList.getState().addChild (Track::createDefaultState(), -1, getUndoManager());

    for (int i = 0; i < midiFile.getNumTracks(); ++i)
    {
        MidiMessageSequence trackSequence (*midiFile.getTrack (i));
        for (MidiMessageSequence::MidiEventHolder* event : trackSequence)
        {
            event->message.setTimeStamp ((event->message.getTimeStamp() / midiFile.getTimeFormat()) + timeOffset);
        }

        int destinationTrack = std::max (0, i + trackOffset);
        Track* importTargetTrack = trackList.tracks[destinationTrack];
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
    for (Track* track : trackList.tracks)
    {
        track->prepareToPlay (samplesPerBlockExpected, sampleRate);
    }
}

void Edit::releaseResources()
{
    keyboardState.reset();
    masterTrack.releaseResources();
    const TrackArray::ScopedLockType sl (trackList.tracks.getLock());
    for (Track* track : trackList.tracks)
    {
        track->releaseResources();
    }
}

void Edit::getNextAudioBlockWithInputs (AudioBuffer<float>& audioBuffer,
        MidiBuffer& incomingMidiBuffer,
        PlaybackEngine& playbackSource)
{
    keyboardState.processNextMidiBuffer (incomingMidiBuffer, 0, audioBuffer.getNumSamples(), true);
    const TrackArray::ScopedLockType sl (trackList.tracks.getLock());
    for (Track* track : trackList.tracks)
    {
        track->getNextAudioBlockWithInputs (audioBuffer, incomingMidiBuffer, playbackSource);
    }
}

void Edit::convertTimestampsFromBeatsToTicks (MidiMessageSequence& sequence) const
{
    for (MidiMessageSequence::MidiEventHolder* event : sequence)
    {
        double newTimestamp = event->message.getTimeStamp() * Transport::pulsesPerQuarterNote;
        event->message.setTimeStamp (newTimestamp);
    }
}

void Edit::saveState()
{
    if (state.exists())
    {
        ValueTree currentState = getState();
        XmlElement* xml = currentState.createXml();
        xml->writeToFile (state, "");
        delete xml;
    }
}

void Edit::newProject()
{
    FileChooser fileChooser ("New Project");
    fileChooser.browseForFileToSave (true);
    state = fileChooser.getResult();
    state.create();
}

File Edit::openProject()
{
    FileChooser fileChooser ("Open Project");
    fileChooser.browseForFileToOpen();
    File file = fileChooser.getResult();
    XmlElement* e = XmlDocument::parse (file);
    ValueTree valueTree = ValueTree::fromXml (*e);
    delete e;
    HarmonaizeApplication::getApp().editWindow->setEdit(valueTree);
    return file;
}

void Edit::changeFile(File file)
{
    state = file;
}
