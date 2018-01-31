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
#define DECLARE_ID(name) const Identifier name (#name);

    DECLARE_ID (Edit)
    namespace EditProps
    {
        DECLARE_ID (OriginTime)
        DECLARE_ID (EndTime)
        DECLARE_ID (SampleRate)
    }

    DECLARE_ID (Transport)
    namespace TransportProps
    {
        DECLARE_ID (PlayPositionTime)
        DECLARE_ID (LoopStartTime)
        DECLARE_ID (LoopEndTime)
        DECLARE_ID (IsLooping)
        DECLARE_ID (PlayState)
    }

    DECLARE_ID (MasterTrack)
    namespace MasterTrackProps
    {
        DECLARE_ID (BeatsPerMinute)
        DECLARE_ID (TimeSigNumerator)
        DECLARE_ID (TimeSigDenominator)
        DECLARE_ID (PulsesPerQuarterNote)
    }

    DECLARE_ID (Track)
    namespace TrackProps
    {
        DECLARE_ID (Name)
        DECLARE_ID (Color)
        DECLARE_ID (Type)
        namespace Types
        {
            DECLARE_ID (MidiSequence)
        }
    }

    DECLARE_ID (Clip)
    namespace ClipProps
    {
        DECLARE_ID (Start)
        DECLARE_ID (Length)
        DECLARE_ID (Color)
        DECLARE_ID (Name)
        DECLARE_ID (Type)
        namespace Types
        {
            DECLARE_ID (Midi)
        }
    }

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
