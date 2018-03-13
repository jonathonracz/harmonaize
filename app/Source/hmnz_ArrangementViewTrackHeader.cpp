/*
  ==============================================================================

    hmnz_ArrangementViewTrackHeader.cpp
    Created: 7 Mar 2018 4:47:06pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_ArrangementViewTrackHeader.h"
#include "hmnz_Track.h"

ArrangementViewTrackHeader::ArrangementViewTrackHeader (Track* _track)
    : track (_track)
{
}

void ArrangementViewTrackHeader::paint (Graphics& g)
{
    g.fillAll (track->color);
}
