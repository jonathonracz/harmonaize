/*
  ==============================================================================

    hmnz_ArrangementViewComponent.cpp
    Created: 7 Mar 2018 1:17:01pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_ArrangementViewComponent.h"
#include "hmnz_Edit.h"

Edit* ArrangementViewComponent::getEdit() const noexcept
{
    return edit;
}

void ArrangementViewComponent::setEdit (Edit* _edit) noexcept
{
    if (edit)
        edit->getState().removeListener (this);

    edit = _edit;

    if (edit)
        edit->getState().addListener (this);

    editChanged();
}

NormalisableRange<double> ArrangementViewComponent::getBeatRemapper() const noexcept
{
    if (!edit)
        return NormalisableRange<double>();

    return NormalisableRange<double> (edit->arrangementViewModel.timeStart, edit->arrangementViewModel.timeEnd);
}

int ArrangementViewComponent::getXPosForBeat (double beat) const noexcept
{
    if (!edit)
        return -1;

    return static_cast<int> (getBeatRemapper().convertTo0to1 (beat) * getWidth());
}

double ArrangementViewComponent::getBeatForXPos (int xPos) const noexcept
{
    if (!edit)
        return 0.0;

    return  getBeatRemapper().convertFrom0to1 (xPos / static_cast<double> (getWidth()));
}

double ArrangementViewComponent::getLinesPerBeatForMinimumLineSpacing (int minimumLineSpacing) const noexcept
{
    if (!edit)
        return 0.0;

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
