/*
  ==============================================================================

    hmnz_ArrangementViewTrackLane.cpp
    Created: 9 Mar 2018 2:36:41pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_ArrangementViewTrackLane.h"
#include "hmnz_ArrangementViewTrackLaneClip.h"
#include "hmnz_Track.h"

ArrangementViewTrackLane::ArrangementViewTrackLane (Track* _track)
    : track (_track)
{
    track->clipList.clips.addListener (this);
}

ArrangementViewTrackLaneClip* ArrangementViewTrackLane::getChildForClip (Clip* clip) noexcept
{
    for (Component* comp : getChildren())
    {
        if (ArrangementViewTrackLaneClip* clipComp = dynamic_cast<ArrangementViewTrackLaneClip*> (comp))
        {
            if (clipComp->getRepresentedClip() == clip)
                return clipComp;
        }
    }

    return nullptr;
}

void ArrangementViewTrackLane::paint (Graphics& g) noexcept
{
    Array<Clip*> clipsToDraw = track->clipList.getClipsForInterval (getBeatForXPos (0), getBeatForXPos (getWidth()));
    g.fillAll (track->color);
}

void ArrangementViewTrackLane::objectAdded (Clip* clip, int, HomogeneousValueTreeObjectArray<Clip>*)
{
    addAndMakeVisible (new ArrangementViewTrackLaneClip (clip));
}

void ArrangementViewTrackLane::objectRemoved (Clip* clip, int, HomogeneousValueTreeObjectArray<Clip>*)
{
    ArrangementViewTrackLaneClip* childToRemove = getChildForClip (clip);
    jassert (childToRemove);
    removeChildComponent (childToRemove);
}
