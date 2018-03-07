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

    int beatBarHeight = 28;
    int arrangementHeight = getHeight() - beatBarHeight;

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

    DBG ("linesPerBeat: " << linesPerBeat);
    double beatValue = Utility::floorToNearestInterval (remapper.start, linesPerBeat);
    while (beatValue <= remapper.end)
    {
        TimeSignature& timeSignature = edit->masterTrack.timeSignature;
        const double beatsInBar = timeSignature.getNumeratorAtBeat (beatValue);
        const double bar = timeSignature.barForBeat (beatValue);
        const double beat = std::floor (timeSignature.beatInBar (beatValue));
        const double subBeat = std::fmod (beatValue, 1.0);

        String beatText = String (bar + 1);
        if (linesPerBeat <= 1)
            beatText += "." + String (beat + 1);
        if (linesPerBeat <= 0.0625)
            beatText += "." + String ((timeSignature.getDenominatorAtBeat (beatValue) * subBeat) + 1);

        bool drawText = false;
        if ((linesPerBeat > 2.0 && std::fmod (bar, 2.0) == 0.0) || // Draw every other measure marker when linesPerBeat > 2.0
            (linesPerBeat <= 2.0 && beat == 0.0 && subBeat == 0.0) || // Always draw measure markers
            (linesPerBeat <= 1.0 && beat == beatsInBar / 2.0 && subBeat == 0) ||
            (linesPerBeat <= 0.25 && subBeat == 0) ||
            (linesPerBeat <= 0.0625 && subBeat == 0.5) ||
            (linesPerBeat <= 0.015625 && std::fmod (subBeat, 0.25) == 0.0))
            drawText = true;

        Colour color = (beat == 0.0 && subBeat == 0.0) ? Colours::white : Colours::grey;
        g.setColour (color);

        int xPos = xPosForBeat (beatValue);
        if (drawText)
        {
            g.drawText (beatText, Rectangle<int> (xPos, 0, getWidth(), beatBarHeight), Justification::Flags::centredLeft, false);
            g.fillRect (Rectangle<int> (xPos, static_cast<int> (beatBarHeight * 0.75), 1, static_cast<int> (beatBarHeight * 0.25)));
        }
        g.fillRect (Rectangle<int> (xPos, beatBarHeight, 1, arrangementHeight));
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

    scaleFactor = 1.0f / scaleFactor; // Invert the scale.
    ArrangementViewModel& model = edit->arrangementViewModel;
    NormalisableRange<double> remapper (model.timeStart, model.timeEnd, 1.0f);
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
