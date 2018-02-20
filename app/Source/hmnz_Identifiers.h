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

        DECLARE_ID (LoopStartBeat)
        DECLARE_ID (LoopEndBeat)
        DECLARE_ID (LoopEnabled)
        DECLARE_ID (RecordEnabled)
        DECLARE_ID (PlayState)
        DECLARE_ID (SampleRate)
        DECLARE_ID (PulsesPerQuarterNote)
    }

    DECLARE_ID (MasterTrack)

    DECLARE_ID (Tempo)

    DECLARE_ID (TimeSignature)

    DECLARE_ID (TimeSigNumerator)
    DECLARE_ID (TimeSigDenominator)

    DECLARE_ID (KeySignature)

    DECLARE_ID (KeySigNumberOfSharpsOrFlats)
    DECLARE_ID (KeySigIsMinor)

    DECLARE_ID (Track)
    namespace TrackProps
    {
        DECLARE_ID (Name)
        DECLARE_ID (Color)
        DECLARE_ID (Type)
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

    DECLARE_ID (Automation)
    DECLARE_ID (AutomationMarker)
    namespace AutomationMarkerProps
    {
        DECLARE_ID (Beat)
        DECLARE_ID (Value)
        DECLARE_ID (Type)
    }

#undef DECLARE_ID
}
