/*
  ==============================================================================

    hmnz_Track.cpp
    Created: 28 Jan 2018 12:32:09pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_Track.h"
#include "hmnz_Edit.h"

Track::Track (const ValueTree& v, UndoManager* um, Edit* const _edit)
    : ValueTreeObject (v, um), edit (_edit), clips (this)
{
}

void Track::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{

}

void Track::releaseResources()
{

}

void Track::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{

}