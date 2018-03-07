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

void ArrangementView::setEdit (Edit* _edit)
{
    if (edit)
        edit->getState().removeListener (this);

    edit = _edit;

    if (edit)
        edit->getState().addListener (this);
}

int ArrangementView::xPosForBeat (double beat) const noexcept
{
    if (!edit)
        return -1;

    NormalisableRange<double> remapper (edit->arrangementViewModel.timeStart, edit->arrangementViewModel.timeEnd);
    return static_cast<int> (remapper.convertTo0to1 (beat) * getWidth());
}

void ArrangementView::resized() noexcept
{
}

void ArrangementView::paint (Graphics& g) noexcept
{
    if (!edit)
        return;

    ArrangementViewModel& model = edit->arrangementViewModel;
    NormalisableRange<double> remapper (model.timeStart, model.timeEnd);
    const double minimumLineSpacing = 14.0; // Arbitrary - controls how close lines can get before the grid size increases
    double linesPerBeat = 1.0;

    {
        double beatPixelDelta = getWidth() / remapper.getRange().getLength();
        while (beatPixelDelta >= minimumLineSpacing * 2.0 && linesPerBeat >= 1.0 / Transport::pulsesPerQuarterNote)
        {
            beatPixelDelta /= 2.0;
            linesPerBeat = std::max (linesPerBeat / 2.0, 1.0 / Transport::pulsesPerQuarterNote);
        }

        while (beatPixelDelta < minimumLineSpacing)
        {
            beatPixelDelta *= 2.0;
            linesPerBeat *= 2.0;
        }
    }

    double beatValue = Utility::ceilToNearestInterval (remapper.start, linesPerBeat);
    while (beatValue <= remapper.end)
    {
        Colour color = (edit->masterTrack.timeSignature.beatInBar (beatValue) > 0.0) ? Colours::grey : Colours::white;
        g.setColour (color);
        int xPos = xPosForBeat (beatValue);
        g.fillRect (Rectangle<int> (xPos, 0, 1, getHeight()));
        beatValue += linesPerBeat;
    }

    double transportBeat = edit->transport.playHeadBeat;
    if (transportBeat >= model.timeStart)
    {
        g.setColour (Colours::blue);
        int transportPos = xPosForBeat (transportBeat);
        lastPaintedPlayHeadBounds = Rectangle<int> (transportPos, 0, 2, getHeight());
        g.fillRect (lastPaintedPlayHeadBounds);
    }
}

void ArrangementView::mouseWheelMove (const MouseEvent& event, const MouseWheelDetails& wheel)
{
    if (!edit)
        return;

    ArrangementViewModel& model = edit->arrangementViewModel;
    NormalisableRange<double> remapper (model.timeStart, model.timeEnd, 1.0f);
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
    if (!edit)
        return;

    ArrangementViewModel& model = edit->arrangementViewModel;
    NormalisableRange<double> remapper (model.timeStart, model.timeEnd, 1.0f);
    // 1.2 factor: this is an arbitrary number to increase responsiveness of magnifying
    const double magnifyIncrease = 1.05;
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
    if (tree == edit->arrangementViewModel.getState())
    {
        repaint();
    }
    else if (tree == edit->transport.getState())
    {
        if (property == edit->transport.playHeadBeat.getPropertyID())
        {
            Rectangle<int> expectedPlayHeadBounds = Rectangle<int> (xPosForBeat (edit->transport.playHeadBeat.get()), 0, 2, getHeight());
            repaint (expectedPlayHeadBounds.getUnion (lastPaintedPlayHeadBounds));
        }
    }
}
