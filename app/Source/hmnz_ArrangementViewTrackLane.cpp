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

ArrangementViewTrackLane::ArrangementViewTrackLane (Track& _track)
    : ArrangementViewTimelineComponent (_track.edit), track (_track)
{
    track.clipList.clips.addListener (this);
    track.getState().addListener (this);
    track.edit.arrangementViewModel.getState().addListener (this);

    for (int i = 0; i < track.clipList.clips.size(); ++i)
        objectAdded (track.clipList.clips[i], i, &(track.clipList.clips));
}

ArrangementViewTrackLane::~ArrangementViewTrackLane()
{
    track.clipList.clips.removeListener (this);
    track.getState().removeListener (this);
    track.edit.arrangementViewModel.getState().removeListener (this);
}

ArrangementViewTrackLaneClip* ArrangementViewTrackLane::getChildForClip (Clip* clip)
{
    for (Component* comp : getChildren())
    {
        if (ArrangementViewTrackLaneClip* clipComp = dynamic_cast<ArrangementViewTrackLaneClip*> (comp))
        {
            if (&clipComp->getRepresentedClip() == clip)
                return clipComp;
        }
    }

    return nullptr;
}

void ArrangementViewTrackLane::updateClipComponentBounds (ArrangementViewTrackLaneClip* clip)
{
    clip->updateBounds();
}

void ArrangementViewTrackLane::updateAllClipComponentBounds()
{
    for (ArrangementViewTrackLaneClip* clip : clips)
        updateClipComponentBounds (clip);
}

void ArrangementViewTrackLane::resized()
{
    updateAllClipComponentBounds();
}

void ArrangementViewTrackLane::objectAdded (Clip* clip, int, HomogeneousValueTreeObjectArray<Clip>*)
{
    ArrangementViewTrackLaneClip* newClip = new ArrangementViewTrackLaneClip (*clip);
    clips.add (newClip);
    addChildComponent (newClip);
    updateClipComponentBounds (newClip);
    newClip->setVisible (true);
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
    if (property == track.edit.arrangementViewModel.timeEnd.getPropertyID() ||
        property == track.edit.arrangementViewModel.timeStart.getPropertyID())
    {
        updateAllClipComponentBounds();
        repaint();
    }
}
