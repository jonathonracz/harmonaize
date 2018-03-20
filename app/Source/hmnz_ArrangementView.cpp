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

ArrangementView::ArrangementView (Edit& _edit)
    : edit (_edit),
      topBar (_edit,
          _edit.arrangementViewModel.timeStart,
          _edit.arrangementViewModel.timeEnd),
      grid (_edit,
          _edit.arrangementViewModel.timeStart,
          _edit.arrangementViewModel.timeEnd),
      headerControls (_edit),
      trackList (_edit),
      playHead (_edit,
          _edit.arrangementViewModel.timeStart,
          _edit.arrangementViewModel.timeEnd),
      headerResizer (_edit)
{
    addAndMakeVisible (topBar);
    addAndMakeVisible (grid);
    addAndMakeVisible (headerControls);
    addAndMakeVisible (trackList);
    addAndMakeVisible (playHead);
    addAndMakeVisible (headerResizer);

    edit.arrangementViewModel.getState().addListener (this);
}

ArrangementView::~ArrangementView()
{
    edit.arrangementViewModel.getState().removeListener (this);
}

void ArrangementView::resized()
{
    const int topBarHeight = 28; // TODO: Hardcoded size constant
    ArrangementViewModel& model = edit.arrangementViewModel;

    FlexBox layout;
    layout.flexDirection = FlexBox::Direction::row;

    FlexBox timelineGroup;
    timelineGroup.flexDirection = FlexBox::Direction::column;
    timelineGroup.items.add (FlexItem (topBar).withHeight (topBarHeight));
    timelineGroup.items.add (FlexItem (grid).withFlex (1.0f));

    FlexBox headerGroup;
    headerGroup.flexDirection = FlexBox::Direction::column;
    headerGroup.items.add (FlexItem (headerControls).withHeight (topBarHeight));
    headerGroup.items.add (FlexItem ().withFlex (1.0f));

    layout.items.add (FlexItem (timelineGroup).withFlex (1.0f));
    layout.items.add (FlexItem (headerGroup).withWidth (model.headerWidth.get()));

    layout.performLayout (getLocalBounds());

    trackList.setBounds (getLocalBounds().withTop (topBar.getHeight()));
    {
        Point<int> playHeadTopLeft = topBar.getBounds().getTopLeft();
        Point<int> playHeadBottomRight = grid.getBounds().getBottomRight();
        playHead.setBounds (Rectangle<int> (playHeadTopLeft, playHeadBottomRight));
    }

    headerResizer.setBounds (grid.getWidth(), 0, 2, getHeight()); // TODO: Hardcoded size constant
}

void ArrangementView::mouseWheelMove (const MouseEvent& event, const MouseWheelDetails& wheel)
{
    ArrangementViewModel& model = edit.arrangementViewModel;
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
    int lastTrackOffset = 0;
    for (int i = 0; i < edit.trackList.tracks.size() - 1; ++i)
        lastTrackOffset += edit.trackList.tracks[i]->height.get();

    float verticalScrollDelta = 20.0f * (wheel.isReversed ? -wheel.deltaY : wheel.deltaY);
    verticalScrollAccumulator += verticalScrollDelta;
    verticalScrollAccumulator = std::max (static_cast<double> (-lastTrackOffset), std::min (0.0, verticalScrollAccumulator));
    model.scrollPosition = static_cast<int> (verticalScrollAccumulator);
}

void ArrangementView::mouseMagnify (const MouseEvent& event, float scaleFactor)
{
    scaleFactor = 1.0f / scaleFactor; // Invert the scale.
    ArrangementViewModel& model = edit.arrangementViewModel;
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

void ArrangementView::valueTreePropertyChanged (ValueTree&, const Identifier& property)
{
    if (property == edit.arrangementViewModel.headerWidth.getPropertyID())
    {
        resized();
        repaint();
    }
}
