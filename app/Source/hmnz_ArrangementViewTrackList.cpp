/*
  ==============================================================================

    hmnz_ArrangementViewTrackList.cpp
    Created: 14 Mar 2018 1:47:29pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_ArrangementViewTrackList.h"
#include "hmnz_Edit.h"

ArrangementViewTrackList::ArrangementViewTrackList (Edit& _edit)
    : edit (_edit)
{
    edit.arrangementViewModel.getState().addListener (this);
    edit.trackList.getState().addListener (this);

    for (int i = 0; i < edit.trackList.tracks.size(); ++i)
        objectAdded (edit.trackList.tracks[i], i, &edit.trackList.tracks);
}

ArrangementViewTrackList::~ArrangementViewTrackList()
{
    edit.arrangementViewModel.getState().removeListener (this);
    edit.trackList.getState().removeListener (this);
}

void ArrangementViewTrackList::resized()
{
    int currentPosition = edit.arrangementViewModel.scrollPosition.get();
    for (int i = 0; i < tracks.size(); ++i)
    {
        tracks[i]->setBounds (0, currentPosition, getWidth(), tracks[i]->getRepresentedTrack().height.get());
        currentPosition += edit.trackList.tracks[i]->height.get();
    }
}

void ArrangementViewTrackList::objectAdded (Track* track, int insertionIndex, HomogeneousValueTreeObjectArray<Track, CriticalSection>*)
{
    ArrangementViewTrack* newTrack = new ArrangementViewTrack (*track);
    tracks.insert (insertionIndex, newTrack);
    addAndMakeVisible (newTrack);
    resized();
    repaint();
}

void ArrangementViewTrackList::objectRemoved (Track* track, int indexRemovedFrom, HomogeneousValueTreeObjectArray<Track, CriticalSection>*)
{
    jassert (&tracks[indexRemovedFrom]->getRepresentedTrack() == track);
    removeChildComponent (tracks[indexRemovedFrom]);
    tracks.remove (indexRemovedFrom);
    resized();
    repaint();
}

void ArrangementViewTrackList::objectOrderChanged (Track* track, int oldIndex, int newIndex, HomogeneousValueTreeObjectArray<Track, CriticalSection>*)
{
    tracks.move (oldIndex, newIndex);
    resized();
    repaint();
}

void ArrangementViewTrackList::valueTreePropertyChanged (ValueTree& tree, const Identifier& property)
{
    if (property == edit.arrangementViewModel.scrollPosition.getPropertyID() ||
        property == IDs::TrackProps::Height)
    {
        // TODO: This could be more elegant
        resized();
        repaint();
    }
}
