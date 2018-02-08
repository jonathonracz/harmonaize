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

ValueTree Edit::createSkeletonEdit()
{
    /*
    float editLength = 60.0;
    float bpm = 120.0;
    float quarterNoteLength = 1.0 / (bpm / 60.0);
     */

    ValueTree edit (IDs::Edit);

    /*
    for (int i = 0; i < 3; ++i)
    {
        auto getColor = [&](int i) -> Colour
        {
            switch (i % 4)
            {
                case 0: return Colours::red;
                case 1: return Colours::green;
                case 2: return Colours::blue;
                case 3: return Colours::yellow;
                default: jassertfalse;
            }

            return Colours::pink;
        };

        ValueTree newTrack (IDs::Track);
        newTrack.setProperty (IDs::TrackProps::Name, "Track " + String (i), nullptr);
        newTrack.setProperty (IDs::TrackProps::Color, static_cast<int64>(getColor (i).getARGB()), nullptr);

        ValueTree newClip (IDs::Clip);
        newClip.setProperty (IDs::ClipProps::Start, 0.0f, nullptr);
        newClip.setProperty (IDs::ClipProps::Length, editLength - (editLength * i), nullptr);
        newClip.setProperty (IDs::ClipProps::Color, static_cast<int64>(getColor (i + 1).getARGB()), nullptr);
        newClip.setProperty (IDs::ClipProps::Type, IDs::ClipProps::Types::Midi.toString(), nullptr);

        int numNotes = 4;
        int velocityInterval = (127 / numNotes + 1);
        for (int i = 0; i < numNotes; ++i)
        {
            ValueTree newNote (IDs::Note);
            newNote.setProperty (IDs::NoteProps::Start, quarterNoteLength * (float)i, nullptr);
            newNote.setProperty (IDs::NoteProps::Length, quarterNoteLength, nullptr);
            newNote.setProperty (IDs::NoteProps::Velocity, velocityInterval + (i * velocityInterval), nullptr);
            newNote.setProperty (IDs::NoteProps::Value, 50 + i, nullptr);
            newClip.addChild (newNote, -1, nullptr);
        }

        newTrack.addChild (newClip, -1, nullptr);
        edit.addChild (newTrack, -1, nullptr);
    } */

    return edit;
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

void Edit::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    masterTrack->getNextAudioBlock (bufferToFill);
    for (Track* track : tracks.objects)
    {
        track->getNextAudioBlock (bufferToFill);
    }
}
