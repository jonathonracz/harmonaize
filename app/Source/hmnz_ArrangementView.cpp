/*
  ==============================================================================

    hmnz_ArrangementView.cpp
    Created: 4 Mar 2018 11:27:08pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_ArrangementView.h"
#include "hmnz_ArrangementViewModel.h"
#include "hmnz_Edit.h"

ArrangementView::ArrangementView()
{
    addAndMakeVisible (topBar);
    addAndMakeVisible (timeline);
}

void ArrangementView::editChanged () noexcept
{
    topBar.setEdit (getEdit());
    timeline.setEdit (getEdit());
}

void ArrangementView::resized() noexcept
{
    const int topBarHeight = 28;
    topBar.setBounds (0, 0, getWidth(), topBarHeight);
    timeline.setBounds (0, topBarHeight, getWidth(), getHeight() - topBarHeight);
}

void ArrangementView::mouseWheelMove (const MouseEvent& event, const MouseWheelDetails& wheel)
{
    if (!getEdit())
        return;

    ArrangementViewModel& model = getEdit()->arrangementViewModel;
    NormalisableRange<double> remapper (model.timeStart, model.timeEnd);
    const double mouseSensitivity = 1.0f / 5.0f;
    double timeToMove = remapper.getRange().getLength() * (wheel.deltaX * mouseSensitivity);
    timeToMove = wheel.isReversed ? -timeToMove : timeToMove;

    if (model.timeStart + timeToMove < 0)
        timeToMove = -model.timeStart;

    if (timeToMove != 0.0)
    {
        model.timeStart = model.timeStart.get() + timeToMove;
        model.timeEnd = model.timeEnd.get() + timeToMove;
    }
}

void ArrangementView::mouseMagnify (const MouseEvent& event, float scaleFactor)
{
    if (!getEdit())
        return;

    scaleFactor = 1.0f / scaleFactor; // Invert the scale.
    ArrangementViewModel& model = getEdit()->arrangementViewModel;
    NormalisableRange<double> remapper (model.timeStart, model.timeEnd);
    // Factor: this is an arbitrary number to change the responsiveness of magnifying.
    const double magnifyIncrease = 1.0;
    double newAmountOfTimeToAdd = remapper.getRange().getLength() * scaleFactor * (scaleFactor >= 1 ? magnifyIncrease : 1.0f / magnifyIncrease) - remapper.getRange().getLength();
    Point<float> relativeMousePosition = event.getEventRelativeTo (this).position / Point<float> (getWidth(), getHeight());
    double newStart = std::max (model.timeStart.get() - (newAmountOfTimeToAdd * relativeMousePosition.x), 0.0);
    double newEnd = model.timeEnd.get() + (newAmountOfTimeToAdd * (1.0 - relativeMousePosition.x));
    if (newEnd - newStart > 0)
    {
        model.timeStart = newStart;
        model.timeEnd = newEnd;
    }
}

void ArrangementView::valueTreePropertyChanged (ValueTree& tree, const Identifier& property) noexcept
{
    if (tree == getEdit()->arrangementViewModel.getState())
    {
        repaint();
    }
}
