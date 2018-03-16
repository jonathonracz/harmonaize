/*
  ==============================================================================

    hmnz_ArrangementViewTimelineComponent.cpp
    Created: 7 Mar 2018 1:17:01pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_ArrangementViewTimelineComponent.h"
#include "hmnz_Edit.h"

ArrangementViewTimelineComponent::ArrangementViewTimelineComponent (Edit& _edit)
    : edit (_edit)
{
}

NormalisableRange<double> ArrangementViewTimelineComponent::getBeatRemapper() const
{
    return NormalisableRange<double> (edit.arrangementViewModel.timeStart, edit.arrangementViewModel.timeEnd);
}

int ArrangementViewTimelineComponent::getXPosForBeat (double beat) const
{
    return static_cast<int> (getBeatRemapper().convertTo0to1 (beat) * getWidth());
}

double ArrangementViewTimelineComponent::getBeatForXPos (int xPos) const
{
    return  getBeatRemapper().convertFrom0to1 (xPos / static_cast<double> (getWidth()));
}

double ArrangementViewTimelineComponent::getLinesPerBeatForMinimumLineSpacing (int minimumLineSpacing) const
{
    NormalisableRange<double> remapper = getBeatRemapper();
    double linesPerBeat = 1.0;
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

    return linesPerBeat;
}
