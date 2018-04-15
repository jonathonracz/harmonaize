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

    DEFINE_HINT (Seconds, "Seconds", "")
    DEFINE_HINT (Beat, "Beat", "")
    DEFINE_HINT (Tempo, "Tempo", "")
    DEFINE_HINT (TimeSignature, "Time Signature", "")
    DEFINE_HINT (KeySignature, "Key Signature", "")
    DEFINE_HINT (Genre, "Genre", "")

    DEFINE_HINT (GoToBeginning, "Go To Beginning", "")
    DEFINE_HINT (Play, "Play", "")
    DEFINE_HINT (Record, "Record", "")
    DEFINE_HINT (Reset, "Reset", "")
    DEFINE_HINT (GenerateAccompaniment, "Generate Accompaniment", "")
    DEFINE_HINT (Metronome, "Metronome", "")

    DEFINE_HINT (HintViewer, "Hint Viewer", "")

    DEFINE_HINT (Arrangement, "Arrangement", "")
    DEFINE_HINT (Clip, "Clip", "")
    DEFINE_HINT (TrackHeader, "Track Header", "")
    DEFINE_HINT (LoadInstrument, "Load Instrument", "")

    DEFINE_HINT (VirtualKeyboard, "Virtual Keyboard", "")

#undef DEFINE_HINT
}
