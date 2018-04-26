/*
  ==============================================================================

    hmnz_HintStrings.cpp
    Created: 14 Apr 2018 3:23:37am
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_HintStrings.h"

namespace HintStrings
{
#define DEFINE_HINT(name, displayName, description) \
    namespace Name { const String name (translate (displayName)); } \
    namespace Description { const String name (translate (description)); }

    DEFINE_HINT (Edit, "Edit View", "");

    DEFINE_HINT (Transport, "Transport", "Contains all controls for navigating and editing the project.")

    DEFINE_HINT (Seconds, "Seconds", "Location of the playhead in terms of time.")
    DEFINE_HINT (Beat, "Beat", "Location of the playhead in terms of beat.")
    DEFINE_HINT (Tempo, "Tempo", "Choose how fast the playhead moves in terms of beats per minute.")
    DEFINE_HINT (TimeSignature, "Time Signature", "Choose a time signature for your project. Click to edit.")
    DEFINE_HINT (KeySignature, "Key Signature", "Choose a key for your accompaniment.")
    DEFINE_HINT (Genre, "Genre", "Choose a genre for your accompaniment.")

    DEFINE_HINT (GoToBeginning, "Go To Beginning", "Click to return the playhead to the start of the project.\nShortcut: 'b'")
    DEFINE_HINT (Play, "Play", "Click to hear your project.\nShortcut: 'Space'")
    DEFINE_HINT (Record, "Record", "Click to begin recording a melody.\nShortcut: 'v'")
    DEFINE_HINT (Reset, "Reset", "Click to delete all current work on your project.\nShortcut: 'c'")
    DEFINE_HINT (GenerateAccompaniment, "Generate Accompaniment", "Click have an accompaniment generated.\nShortcut: 'g'")
    DEFINE_HINT (Metronome, "Metronome", "Click to toggle a metronome when you record.\nShortcut: 'm'")

    DEFINE_HINT (HintViewer, "Hint Viewer", "I'm the hint viewer. I'm here to tell you what's what.")

    DEFINE_HINT (Arrangement, "Arrangement", "Visual representation of your existing tracks.")
    DEFINE_HINT (Clip, "Clip", "Visual representation of a single track.")
    DEFINE_HINT (TrackHeader, "Track Header", "Name your tracks. Click to edit.")
    DEFINE_HINT (LoadInstrument, "Load Instrument", "Click to select the track instrument.")

    DEFINE_HINT (VirtualKeyboard, "Virtual Keyboard", "Functional keyboard component. Click to play!")

#undef DEFINE_HINT
}
