/*
  ==============================================================================

    hmnz_Track.cpp
    Created: 28 Jan 2018 12:32:09pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_Track.h"

Track::Track (const ValueTree& v, UndoManager* um, const Edit* _owner)
    : ValueTreeObject (v, um), owner (_owner), clips (this)
{
}

void Track::setNextReadPosition (int64 newPosition)
{
    currentReadPosition = newPosition;
}

int64 Track::getNextReadPosition() const
{
    return currentReadPosition;
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
