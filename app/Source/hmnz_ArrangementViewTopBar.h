/*
  ==============================================================================

    hmnz_ArrangementViewTopBar.h
    Created: 7 Mar 2018 1:05:20pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ArrangementViewComponent.h"

class ArrangementViewTopBar : public ArrangementViewComponent
{
public:
    ArrangementViewTopBar() = default;

private:
    void paint (Graphics&) noexcept override;

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override {}
    void valueTreeChildAdded (ValueTree&, ValueTree&) override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementViewTopBar)
};
