/*
  ==============================================================================

    hmnz_ArrangementViewHeaderResizer.cpp
    Created: 17 Mar 2018 4:13:32am
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_ArrangementViewHeaderResizer.h"
#include "hmnz_Edit.h"

ArrangementViewHeaderResizer::ArrangementViewHeaderResizer (Edit& _edit)
    : edit (_edit)
{
    setMouseCursor (MouseCursor::StandardCursorType::LeftRightResizeCursor);
}

void ArrangementViewHeaderResizer::paint (Graphics& g)
{
    g.fillAll (Colours::white);
}

void ArrangementViewHeaderResizer::mouseDown(const MouseEvent& event)
{
    lastDragPos = 0;
}

void ArrangementViewHeaderResizer::mouseDrag (const MouseEvent& event)
{
    edit.arrangementViewModel.headerWidth = edit.arrangementViewModel.headerWidth.get() + lastDragPos - event.getDistanceFromDragStartX();
    lastDragPos = event.getDistanceFromDragStartX();
}
