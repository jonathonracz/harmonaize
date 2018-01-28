/*
  ==============================================================================

    hmnz_Track.cpp
    Created: 28 Jan 2018 12:32:09pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_Track.h"

Track::Track (const ValueTree& v, UndoManager* um)
    : ValueTreeObject (v, um), clips (v, um)
{

}

void Track::setNextReadPosition (int64 newPosition)
{

}

int64 Track::getNextReadPosition() const
{

}

int64 Track::getTotalLength() const
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
