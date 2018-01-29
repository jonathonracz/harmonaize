/*
  ==============================================================================

    hmnz_Track.cpp
    Created: 28 Jan 2018 12:32:09pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_Track.h"
#include "hmnz_Edit.h"

Track::Track (const ValueTree& v, UndoManager* um, Edit* const _owner)
    : ValueTreeObject (v, um), owner (_owner), clips (this)
{
}

void Track::setNextReadPosition (int64 newPosition)
{
    owner->setNextReadPosition (newPosition);
}

int64 Track::getNextReadPosition() const
{
    return owner->getNextReadPosition();
}

int64 Track::getTotalLength() const
{
    return owner->getTotalLength();
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
