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
#include "hmnz_ArrangementViewModel.h"
#include "hmnz_Edit.h"

ArrangementViewTrackLane::ArrangementViewTrackLane (Track* _track)
    : track (_track)
{
    jassert (track);
    track->clipList.clips.addListener (this);
    track->getState().addListener (this);

    for (int i = 0; i < track->clipList.clips.size(); ++i)
        objectAdded (track->clipList.clips[i], i, &(track->clipList.clips));
}

ArrangementViewTrackLane::~ArrangementViewTrackLane()
{
    if (getEdit())
    {
        getEdit()->arrangementViewModel.getState().removeListener (this);
        track->getState().removeListener (this);
    }
}

void ArrangementViewTrackLane::editChanged (Edit* oldEdit)
{
    clips.clear();
    getEdit()->arrangementViewModel.getState().addListener (this);
}

ArrangementViewTrackLaneClip* ArrangementViewTrackLane::getChildForClip (Clip* clip)
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

void ArrangementViewTrackLane::paint (Graphics& g)
{
    g.fillAll (track->color);
}

void ArrangementViewTrackLane::objectAdded (Clip* clip, int, HomogeneousValueTreeObjectArray<Clip>*)
{
    ArrangementViewTrackLaneClip* newClip = new ArrangementViewTrackLaneClip (clip);
    clips.add (newClip);
    addAndMakeVisible (newClip);
}

void ArrangementViewTrackLane::objectRemoved (Clip* clip, int, HomogeneousValueTreeObjectArray<Clip>*)
{
    ArrangementViewTrackLaneClip* childToRemove = getChildForClip (clip);
    jassert (childToRemove);
    removeChildComponent (childToRemove);
    clips.removeObject (childToRemove);
}

void ArrangementViewTrackLane::valueTreePropertyChanged (ValueTree&, const Identifier& property)
{
    if (property == track->height.getPropertyID())
    {
        for (Component* child : getChildren())
        {
            if (ArrangementViewTrackLaneClip* clip = dynamic_cast<ArrangementViewTrackLaneClip*> (child))
            {
                clip->setBounds (clip->getBounds().withHeight (track->height.get()));
            }
        }
    }
}
