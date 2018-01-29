/*
  ==============================================================================

    hmnz_Edit.cpp
    Created: 28 Jan 2018 12:49:59am
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_Edit.h"

Edit::Edit (const ValueTree& v)
    : ValueTreeObject<IDs::Edit> (v, &undoManager), tracks (this)
{
    setOpaque (true);
    // TODO: Validate the ValueTree data model, display an error if
    // something unexpected occurs, etc...
    masterTrack = std::unique_ptr<MasterTrack>(new MasterTrack (v.getChildWithName (IDs::MasterTrack), &undoManager));
    transport = std::unique_ptr<Transport>(new Transport (v.getChildWithName (IDs::Transport), &undoManager));
    ValueTree state = getState();
    stateDebugger.setSource (state);
}

ValueTree Edit::createSkeletonEdit()
{
    float editLength = 60.0f;
    float bpm = 120.0f;
    float quarterNoteLength = 1.0f / (bpm / 60.0f);

    ValueTree edit (IDs::Edit);
    edit.setProperty (IDs::EditProps::OriginTime, 0.0f, nullptr);
    edit.setProperty (IDs::EditProps::EndTime, editLength, nullptr);
    edit.setProperty (IDs::EditProps::PulsesPerQuarterNote, 960, nullptr);
    edit.setProperty (IDs::EditProps::SampleRate, 44100, nullptr);

    {
        ValueTree transport (IDs::Transport);
        transport.setProperty (IDs::TransportProps::PlayPositionTime, 0.0f, nullptr);
        transport.setProperty (IDs::TransportProps::LoopStartTime, 0.0f, nullptr);
        transport.setProperty (IDs::TransportProps::LoopEndTime, editLength / 4.0f, nullptr);
        transport.setProperty (IDs::TransportProps::IsLooping, false, nullptr);
        edit.addChild (transport, -1, nullptr);
    }

    {
        ValueTree masterTrack (IDs::MasterTrack);
        masterTrack.setProperty (IDs::MasterTrackProps::BeatsPerMinute, bpm, nullptr);
        masterTrack.setProperty (IDs::MasterTrackProps::TimeSigNumerator, 4, nullptr);
        masterTrack.setProperty (IDs::MasterTrackProps::TimeSigDenominator, 4, nullptr);
        edit.addChild (masterTrack, -1, nullptr);
    }

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
        newTrack.setProperty (IDs::TrackProps::Type, IDs::TrackProps::Types::MidiSequence.toString(), nullptr);

        ValueTree newClip (IDs::Clip);
        newClip.setProperty (IDs::ClipProps::Start, 0.0f, nullptr);
        newClip.setProperty (IDs::ClipProps::Length, editLength - (editLength * i), nullptr);
        newClip.setProperty (IDs::ClipProps::Color, static_cast<int64>(getColor (i + 1).getARGB()), nullptr);
        newClip.setProperty (IDs::ClipProps::Name, "Clip for track " + String (i), nullptr);
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
    }

    return edit;
}

void Edit::valueTreePropertyChanged (ValueTree& tree, const Identifier& id)
{
    if (tree == getState())
    {
        if (id == IDs::EditProps::WindowWidth || id == IDs::EditProps::WindowHeight)
        {
            setSize(getState()[IDs::EditProps::WindowWidth], getState()[IDs::EditProps::WindowHeight]);
        }
    }
}

void Edit::resized()
{
}

void Edit::paint (Graphics& g)
{
    g.fillAll(Colours::red);
}

void Edit::setNextReadPosition (int64 newPosition)
{
    currentPlaybackPosition = newPosition;
}

int64 Edit::getNextReadPosition() const
{
    return currentPlaybackPosition;
}

int64 Edit::getTotalLength() const
{
    return (float(endTime.getValue()) - float(originTime.getValue())) * float(sampleRate.getValue());
}

void Edit::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
}

void Edit::releaseResources()
{
}

void Edit::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
}
