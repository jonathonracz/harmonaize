/*
  ==============================================================================

    hmnz_ArrangementViewTrackList.cpp
    Created: 14 Mar 2018 1:47:29pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_ArrangementViewTrackList.h"
#include "hmnz_ArrangementViewTrack.h"
#include "hmnz_Edit.h"

void ArrangementViewTrackList::editChanged (Edit* oldEdit)
{
    if (oldEdit)
        oldEdit->trackList.tracks.removeListener (this);

    removeAllChildren();
    tracks.clear();

    if (getEdit())
    {
        for (int i = 0; i < getEdit()->trackList.tracks.size(); ++i)
        {
            objectAdded (getEdit()->trackList.tracks[i], i, &(getEdit()->trackList.tracks));
        }

        getEdit()->trackList.tracks.addListener (this);
    }
}

void ArrangementViewTrackList::resized()
{
    if (!getEdit())
        return;

    int currentPosition = getEdit()->arrangementViewModel.scrollPosition.get();
    for (int i = 0; i < tracks.size(); ++i)
    {
        tracks[i]->setBounds (0, currentPosition, getWidth(), tracks[i]->getRepresentedTrack()->height.get());
        currentPosition += getEdit()->trackList.tracks[i]->height.get();
    }
}

void ArrangementViewTrackList::objectAdded (Track* track, int insertionIndex, HomogeneousValueTreeObjectArray<Track, CriticalSection>*)
{
    ArrangementViewTrack* newTrack = new ArrangementViewTrack (track);
    tracks.add (newTrack);
    addAndMakeVisible (newTrack);
    repaint();
}

void ArrangementViewTrackList::objectRemoved (Track* track, int indexRemovedFrom, HomogeneousValueTreeObjectArray<Track, CriticalSection>*)
{
    jassert (tracks[indexRemovedFrom]->getRepresentedTrack() == track);
    removeChildComponent (tracks[indexRemovedFrom]);
    tracks.remove (indexRemovedFrom);
    repaint();
}

void ArrangementViewTrackList::objectOrderChanged (Track* track, int oldIndex, int newIndex, HomogeneousValueTreeObjectArray<Track, CriticalSection>*)
{
    tracks.move (oldIndex, newIndex);
    repaint();
}

void ArrangementViewTrackList::valueTreePropertyChanged (ValueTree& tree, const Identifier& property)
{
    if (property == getEdit()->arrangementViewModel.scrollPosition.getPropertyID() ||
        property == IDs::TrackProps::Height)
    {
        // TODO: This could be more elegant
        resized();
        repaint();
    }

}
