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
    addAndMakeVisible (headerList);
}

void ArrangementView::editChanged (Edit* oldEdit) noexcept
{
    topBar.setEdit (getEdit());
    timeline.setEdit (getEdit());
    headerList.setEdit (getEdit());
}

void ArrangementView::resized() noexcept
{
    const int topBarHeight = 28;
    if (!getEdit())
        return;

    ArrangementViewModel& model = getEdit()->arrangementViewModel;

    FlexBox layout;
    layout.flexDirection = FlexBox::Direction::row;

    FlexBox timelineGroup;
    timelineGroup.flexDirection = FlexBox::Direction::column;
    timelineGroup.items.add (FlexItem (topBar).withHeight (topBarHeight));
    timelineGroup.items.add (FlexItem (timeline).withFlex (1.0f));

    FlexBox header;
    header.flexDirection = FlexBox::Direction::column;
    header.items.add (FlexItem().withHeight (topBarHeight));
    header.items.add (FlexItem (headerList).withFlex (1.0f));

    layout.items.add (FlexItem (timelineGroup).withFlex (1.0f));
    layout.items.add (FlexItem (header).withWidth (model.headerWidth.get()));

    layout.performLayout (getLocalBounds());
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

    // Magic number is an arbitrary factor to increase scrolling speed
    float verticalScrollDelta = 20.0f * (wheel.isReversed ? -wheel.deltaY : wheel.deltaY);
    verticalScrollAccumulator += verticalScrollDelta;
    verticalScrollAccumulator = std::max (0.0, verticalScrollAccumulator);
    model.scrollPosition = static_cast<int> (verticalScrollAccumulator);
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
