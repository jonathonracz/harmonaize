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

    editChanged (edit);
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
