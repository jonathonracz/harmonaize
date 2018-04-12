/*
  ==============================================================================

    hmnz_Identifiers.cpp
    Created: 13 Mar 2018 1:39:38pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_Identifiers.h"

namespace IDs
{
#define DEFINE_ID(name) const Identifier name (#name);

    DEFINE_ID (Test)
    DEFINE_ID (TestWithArray)

    DEFINE_ID (Edit)
    namespace EditProps
    {
        DEFINE_ID (SelectedTrack)
    }

    DEFINE_ID (Transport)
    namespace TransportProps
    {
        DEFINE_ID (PlayHeadTime)
        DEFINE_ID (PlayHeadBeat)
        DEFINE_ID (PlayHeadTempo)
        DEFINE_ID (PlayHeadTimeSigNumerator)
        DEFINE_ID (PlayHeadTimeSigDenominator)
        DEFINE_ID (PlayHeadKeySigNumSharpsOrFlats)
        DEFINE_ID (PlayHeadKeySigIsMinor)

        DEFINE_ID (RecordEnabled)
        DEFINE_ID (PlayState)
        DEFINE_ID (SampleRate)
    }

    DEFINE_ID (ArrangementViewModel)
    namespace ArrangementViewModelProps
    {
        DEFINE_ID (TimeStart)
        DEFINE_ID (TimeEnd)
        DEFINE_ID (HeaderWidth)
        DEFINE_ID (ScrollPosition)
    }

    DEFINE_ID (TrackEditViewModel)
    namespace TrackEditViewModelProps
    {
        DEFINE_ID (TimeStart)
        DEFINE_ID (TimeEnd)
        DEFINE_ID (MidiScrollPosition)
    }

    DEFINE_ID (MasterTrack)
    namespace MasterTrackProps
    {
        DEFINE_ID (MetronomeEnabled)
    }

    DEFINE_ID (Tempo)
    namespace TempoProps
    {
        DEFINE_ID (BeatsPerMinute)
    }

    DEFINE_ID (TimeSignature)
    namespace TimeSignatureProps
    {
        DEFINE_ID (Numerator)
        DEFINE_ID (Denominator)
    }

    DEFINE_ID (KeySignature)
    namespace KeySignatureProps
    {
        DEFINE_ID (NumberOfSharpsOrFlats)
        DEFINE_ID (IsMinor)
    }

    DEFINE_ID (Genre)
    namespace GenreProps
    {
        DEFINE_ID (GenreIndex)
    }

    DEFINE_ID (TrackList)

    DEFINE_ID (Track)
    namespace TrackProps
    {
        DEFINE_ID (Name)
        DEFINE_ID (Color)
        DEFINE_ID (Type)
        DEFINE_ID (Height)
        namespace Types
        {
            DEFINE_ID (Midi)
        }
        DEFINE_ID (RecordArmed)
    }

    DEFINE_ID (ClipList)

    DEFINE_ID (Clip)
    namespace ClipProps
    {
        DEFINE_ID (Start)
        DEFINE_ID (Length)
        DEFINE_ID (Color)
        DEFINE_ID (Type)
        namespace Types
        {
            DEFINE_ID (Midi)
        }
    }

    DEFINE_ID (MidiMessageModel)
    namespace MidiMessageModelProps
    {
        DEFINE_ID (Bytes)
        DEFINE_ID (Timestamp)
    }

    DEFINE_ID (MidiMessageSequenceModel)
    namespace MidiMessageSequenceModelProps
    {
    }

    DEFINE_ID (NoteList)

    DEFINE_ID (Note)
    namespace NoteProps
    {
        DEFINE_ID (Start)
        DEFINE_ID (Length)
        DEFINE_ID (Velocity)
        DEFINE_ID (Value)
    }

#undef DEFINE_ID
}
