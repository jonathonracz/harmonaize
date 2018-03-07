/*
  ==============================================================================

    hmnz_ArrangementViewComponent.h
    Created: 7 Mar 2018 1:17:01pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"

class Edit;

class ArrangementViewComponent  : public Component,
                                  public ValueTree::Listener
{
public:
    ArrangementViewComponent() = default;

    Edit* getEdit() const noexcept;
    void setEdit (Edit* edit) noexcept;

protected:
    virtual void editChanged (Edit* newEdit) noexcept {}
    NormalisableRange<double> getBeatRemapper() const noexcept;
    int getXPosForBeat (double beat) const noexcept;

private:
    WeakReference<Edit> edit;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementViewComponent)
};
