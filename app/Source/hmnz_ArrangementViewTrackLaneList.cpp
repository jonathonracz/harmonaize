/*
  ==============================================================================

    hmnz_ArrangementViewTrackLaneList.cpp
    Created: 9 Mar 2018 2:36:58pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_ArrangementViewTrackLaneList.h"
#include "hmnz_Edit.h"

void ArrangementViewTrackLaneList::editChanged (Edit* oldEdit)
{
    if (oldEdit)
        oldEdit->trackList.tracks.removeListener (this);

    removeAllChildren();

    if (getEdit())
    {
        for (int i = 0; i < getEdit()->trackList.tracks.size(); ++i)
        {
            objectAdded (getEdit()->trackList.tracks[i], i, &(getEdit()->trackList.tracks));
        }

        getEdit()->trackList.tracks.addListener (this);
    }
}

void ArrangementViewTrackLaneList::resized()
{
    if (!getEdit())
        return;

    int currentPosition = getEdit()->arrangementViewModel.scrollPosition.get();
    for (int i = 0; i < lanes.size(); ++i)
    {
        lanes[i]->setBounds (0, currentPosition, getWidth(), lanes[i]->getRepresentedTrack()->height.get());
    }
}

void ArrangementViewTrackLaneList::objectAdded (Track* track, int insertionIndex, HomogeneousValueTreeObjectArray<Track, CriticalSection>*)
{
    ArrangementViewTrackLane* newLane = new ArrangementViewTrackLane (track);
    lanes.add (newLane);
    addAndMakeVisible (newLane);
}

void ArrangementViewTrackLaneList::objectRemoved (Track* track, int indexRemovedFrom, HomogeneousValueTreeObjectArray<Track, CriticalSection>*)
{
    jassert (lanes[indexRemovedFrom]->getRepresentedTrack() == track);
    removeChildComponent (lanes[indexRemovedFrom]);
    lanes.remove (indexRemovedFrom);
}

void ArrangementViewTrackLaneList::objectOrderChanged (Track* track, int oldIndex, int newIndex, HomogeneousValueTreeObjectArray<Track, CriticalSection>*)
{
    lanes.move (oldIndex, newIndex);
}

void ArrangementViewTrackLaneList::valueTreePropertyChanged (ValueTree& tree, const Identifier& property)
{
    if (tree == getEdit()->arrangementViewModel.getState())
    {
        if (property == getEdit()->arrangementViewModel.scrollPosition.getPropertyID())
        {
            // TODO: This could be more elegant
            resized();
            repaint();
        }
    }
}
