/*
  ==============================================================================

    hmnz_ArrangementViewTrackHeaderList.cpp
    Created: 7 Mar 2018 4:14:01pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_ArrangementViewTrackHeaderList.h"
#include "hmnz_Edit.h"

void ArrangementViewTrackHeaderList::editChanged (Edit* oldEdit)
{
    if (oldEdit)
        oldEdit->trackList.tracks.removeListener (this);

    removeAllChildren();
    headers.clear();

    if (getEdit())
    {
        for (int i = 0; i < getEdit()->trackList.tracks.size(); ++i)
        {
            objectAdded (getEdit()->trackList.tracks[i], i, &(getEdit()->trackList.tracks));
        }
        
        getEdit()->trackList.tracks.addListener (this);
    }
}

void ArrangementViewTrackHeaderList::resized()
{
    if (!getEdit())
        return;

    int currentPosition = getEdit()->arrangementViewModel.scrollPosition.get();
    for (int i = 0; i < headers.size(); ++i)
    {
        headers[i]->setBounds (0, currentPosition, getWidth(), headers[i]->getRepresentedTrack()->height.get());
        currentPosition += getEdit()->trackList.tracks[i]->height.get();
    }
}

void ArrangementViewTrackHeaderList::objectAdded (Track* track, int insertionIndex, HomogeneousValueTreeObjectArray<Track, CriticalSection>*)
{
    ArrangementViewTrackHeader* newHeader = new ArrangementViewTrackHeader (track);
    headers.add (newHeader);
    addAndMakeVisible (newHeader);
    repaint();
}

void ArrangementViewTrackHeaderList::objectRemoved (Track* track, int indexRemovedFrom, HomogeneousValueTreeObjectArray<Track, CriticalSection>*)
{
    jassert (headers[indexRemovedFrom]->getRepresentedTrack() == track);
    removeChildComponent (headers[indexRemovedFrom]);
    headers.remove (indexRemovedFrom);
    repaint();
}

void ArrangementViewTrackHeaderList::objectOrderChanged (Track* track, int oldIndex, int newIndex, HomogeneousValueTreeObjectArray<Track, CriticalSection>*)
{
    headers.move (oldIndex, newIndex);
    repaint();
}

void ArrangementViewTrackHeaderList::valueTreePropertyChanged (ValueTree& tree, const Identifier& property)
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
