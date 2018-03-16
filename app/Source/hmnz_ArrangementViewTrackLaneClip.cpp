/*
  ==============================================================================

    hmnz_ArrangementViewTrackLaneClip.cpp
    Created: 11 Mar 2018 12:03:40pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_ArrangementViewTrackLaneClip.h"
#include "hmnz_Clip.h"

ArrangementViewTrackLaneClip::ArrangementViewTrackLaneClip (Clip& _clip)
    : clip (_clip)
{
    clip.getState().addListener (this);
}

ArrangementViewTrackLaneClip::~ArrangementViewTrackLaneClip()
{
    clip.getState().removeListener (this);
}

void ArrangementViewTrackLaneClip::updateBounds()
{
    ArrangementViewTimelineComponent* timelineParent = static_cast<ArrangementViewTimelineComponent*> (getParentComponent());
    double start = getRepresentedClip().start.get();
    double length = getRepresentedClip().length.get();
    Rectangle<int> bounds (Point<int> (timelineParent->getXPosForBeat (start), 0), Point<int> (timelineParent->getXPosForBeat (start + length), timelineParent->getHeight()));
    setBounds (bounds);
}

void ArrangementViewTrackLaneClip::paint (Graphics& g)
{
    g.fillAll (Colours::white);
}

void ArrangementViewTrackLaneClip::valueTreePropertyChanged (ValueTree&, const Identifier& property)
{
    if (property == clip.start.getPropertyID() ||
        property == clip.length.getPropertyID())
    {
        updateBounds();
    }
}
