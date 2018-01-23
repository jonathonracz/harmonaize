/*
  ==============================================================================

    hmnz_MidiImporter.h
    Created: 22 Jan 2018 10:14:59pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_Edit.h"

class MidiImporter
{
public:
    MidiImporter (Edit& edit)
        : importDest (edit) {}

    ValueTree import (const File& midiFile)
    {
        FileInputStream* inputStream = midiFile.createInputStream();

    }

    ValueTree import (const MidiFile& midiFile)
    {

    }

private:
    Edit& importDest;
};
