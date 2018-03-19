/*
  ==============================================================================

    hmnz_ArrangementViewTrackResizer.cpp
    Created: 17 Mar 2018 3:55:57am
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_ArrangementViewTrackResizer.h"
#include "hmnz_Track.h"

ArrangementViewTrackResizer::ArrangementViewTrackResizer (Track& _track)
    : track (_track)
{
    setMouseCursor (MouseCursor::StandardCursorType::UpDownResizeCursor);
}

void ArrangementViewTrackResizer::paint (Graphics& g)
{
    g.fillAll (Colours::white);
}

void ArrangementViewTrackResizer::mouseDown(const MouseEvent& event)
{
    lastDragPos = 0;
}

void ArrangementViewTrackResizer::mouseDrag (const MouseEvent& event)
{
    track.height = track.height.get() + event.getDistanceFromDragStartY() - lastDragPos;
    lastDragPos = event.getDistanceFromDragStartY();
}
