/*
  ==============================================================================

    hmnz_ArrangementViewTopBar.cpp
    Created: 7 Mar 2018 1:05:20pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_ArrangementViewTopBar.h"
#include "hmnz_Edit.h"
#include "hmnz_Application.h"

ArrangementViewTopBar::ArrangementViewTopBar (Edit& _edit, CachedValue<double>& timeStart, CachedValue<double>& timeEnd)
    : ArrangementViewTimelineComponent (timeStart, timeEnd), edit (_edit)
{
    timeStart.getValueTree().addListener (this);
    timeEnd.getValueTree().addListener (this);
}

ArrangementViewTopBar::~ArrangementViewTopBar()
{
    timeStart.getValueTree().removeListener (this);
    timeEnd.getValueTree().removeListener (this);
}

void ArrangementViewTopBar::paint (Graphics& g)
{
    NormalisableRange<double> remapper (timeStart.get(), timeEnd.get());
    const int minimumLineSpacing = 14; // Arbitrary - controls how close lines can get before the grid size increases
    double linesPerBeat = getLinesPerBeatForMinimumLineSpacing (minimumLineSpacing);

    double beatValue = Utility::floorToNearestInterval (remapper.start, linesPerBeat);
    while (beatValue <= remapper.end)
    {
        const TimeSignature& timeSignature = edit.masterTrack.timeSignature;
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

        int xPos = getXPosForBeat (beatValue);
        if (drawText)
        {
            EditView* editView = static_cast<EditView*>(HarmonaizeApplication::getApp().editWindow.get()->getContentComponent());
            double fontSize = editView->customLookAndFeel->getFontScale();
            g.setFont (15 * fontSize);
            g.drawText (beatText, Rectangle<int> (xPos, 0, getWidth(), getHeight()), Justification::Flags::centredLeft, false);
            g.fillRect (Rectangle<int> (xPos, static_cast<int> (getHeight() * 0.75), 1, static_cast<int> (getHeight() * 0.25)));
        }
        beatValue += linesPerBeat;
    }
}

void ArrangementViewTopBar::mouseDown (const MouseEvent& event)
{
    edit.transport.playHeadBeat = getBeatForXPos (static_cast<int> (event.position.x));
}

void ArrangementViewTopBar::mouseDrag (const MouseEvent& event)
{
    int position = std::max (std::min (getWidth(), static_cast<int> (event.position.x)), 0);
    edit.transport.playHeadBeat = getBeatForXPos (position);
}

void ArrangementViewTopBar::valueTreePropertyChanged (ValueTree&, const Identifier& property)
{
    if (property == timeStart.getPropertyID() ||
        property == timeEnd.getPropertyID())
    {
        repaint();
    }
}
