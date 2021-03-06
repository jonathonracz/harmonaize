/*
  ==============================================================================

    hmnz_Identifiers.h
    Created: 22 Jan 2018 3:17:40pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

namespace IDs
{
#define DECLARE_ID(name) extern const Identifier name;

    DECLARE_ID (Test)
    DECLARE_ID (TestWithArray)

    DECLARE_ID (Edit)
    namespace EditProps
    {
        DECLARE_ID (SelectedTrack)
    }

    DECLARE_ID (Transport)
    namespace TransportProps
    {
        DECLARE_ID (PlayHeadTime)
        DECLARE_ID (PlayHeadBeat)
        DECLARE_ID (PlayHeadTempo)
        DECLARE_ID (PlayHeadTimeSigNumerator)
        DECLARE_ID (PlayHeadTimeSigDenominator)
        DECLARE_ID (PlayHeadKeySigNumSharpsOrFlats)
        DECLARE_ID (PlayHeadKeySigIsMinor)

        DECLARE_ID (RecordEnabled)
        DECLARE_ID (PlayState)
        DECLARE_ID (SampleRate)
    }

    DECLARE_ID (ArrangementViewModel)
    namespace ArrangementViewModelProps
    {
        DECLARE_ID (TimeStart)
        DECLARE_ID (TimeEnd)
        DECLARE_ID (HeaderWidth)
        DECLARE_ID (ScrollPosition)
    }

    DECLARE_ID (TrackEditViewModel)
    namespace TrackEditViewModelProps
    {
        DECLARE_ID (TimeStart)
        DECLARE_ID (TimeEnd)
        DECLARE_ID (MidiScrollPosition)
    }

    DECLARE_ID (MasterTrack)
    namespace MasterTrackProps
    {
        DECLARE_ID (MetronomeEnabled)
    }

    DECLARE_ID (Tempo)
    namespace TempoProps
    {
        DECLARE_ID (BeatsPerMinute)
    }

    DECLARE_ID (TimeSignature)
    namespace TimeSignatureProps
    {
        DECLARE_ID (Numerator)
        DECLARE_ID (Denominator)
    }

    DECLARE_ID (KeySignature)
    namespace KeySignatureProps
    {
        DECLARE_ID (NumberOfSharpsOrFlats)
        DECLARE_ID (IsMinor)
    }

    DECLARE_ID (Genre)
    namespace GenreProps
    {
        DECLARE_ID (GenreIndex)
    }

    DECLARE_ID (TrackList)

    DECLARE_ID (Track)
    namespace TrackProps
    {
        DECLARE_ID (Name)
        DECLARE_ID (Color)
        DECLARE_ID (Type)
        DECLARE_ID (Height)
        namespace Types
        {
            DECLARE_ID (Midi)
        }
        DECLARE_ID (RecordArmed)
    }

    DECLARE_ID (ClipList)

    DECLARE_ID (Clip)
    namespace ClipProps
    {
        DECLARE_ID (Start)
        DECLARE_ID (Length)
        DECLARE_ID (Color)
        DECLARE_ID (Type)
        namespace Types
        {
            DECLARE_ID (Midi)
        }
    }

    DECLARE_ID (MidiMessageModel)
    namespace MidiMessageModelProps
    {
        DECLARE_ID (Bytes)
        DECLARE_ID (Timestamp)
    }

    DECLARE_ID (MidiMessageSequenceModel)
    namespace MidiMessageSequenceModelProps
    {
    }

    DECLARE_ID (NoteList)

    DECLARE_ID (Note)
    namespace NoteProps
    {
        DECLARE_ID (Start)
        DECLARE_ID (Length)
        DECLARE_ID (Velocity)
        DECLARE_ID (Value)
    }

#undef DECLARE_ID
}
