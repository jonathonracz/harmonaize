/*
  ==============================================================================

    hmnz_ArrangementViewTrackHeader.cpp
    Created: 7 Mar 2018 4:47:06pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_ArrangementViewTrackHeader.h"
#include "hmnz_Track.h"

ArrangementViewTrackHeader::ArrangementViewTrackHeader (Track& _track)
    : track (_track)
{
    track.getState().addListener (this);
}

ArrangementViewTrackHeader::~ArrangementViewTrackHeader()
{
    track.getState().removeListener (this);
}

void ArrangementViewTrackHeader::paint (Graphics& g)
{
    g.fillAll (track.color);
}

void ArrangementViewTrackHeader::valueTreePropertyChanged (ValueTree&, const Identifier& property)
{

}
