/*
  ==============================================================================

    hmnz_HintStrings.h
    Created: 14 Apr 2018 3:23:37am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

namespace HintStrings
{
#define DECLARE_HINT(name) \
    namespace Name { extern const String name; } \
    namespace Description { extern const String name; }

    DECLARE_HINT (Edit)

    DECLARE_HINT (Transport)

    DECLARE_HINT (Seconds)
    DECLARE_HINT (Beat)
    DECLARE_HINT (Tempo)
    DECLARE_HINT (TimeSignature)
    DECLARE_HINT (KeySignature)
    DECLARE_HINT (Genre)

    DECLARE_HINT (GoToBeginning)
    DECLARE_HINT (Play)
    DECLARE_HINT (Record)
    DECLARE_HINT (Reset)
    DECLARE_HINT (GenerateAccompaniment)
    DECLARE_HINT (Metronome)

    DECLARE_HINT (HintViewer)

    DECLARE_HINT (Arrangement)
    DECLARE_HINT (Clip)
    DECLARE_HINT (TrackHeader)
    DECLARE_HINT (LoadInstrument)

    DECLARE_HINT (VirtualKeyboard)

#undef DECLARE_HINT
}
