/*
  ==============================================================================

    hmnz_ArrangementView.h
    Created: 4 Mar 2018 11:27:08pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ArrangementViewTopBar.h"
#include "hmnz_ArrangementViewTimeline.h"

class ArrangementView   : public ArrangementViewComponent
{
public:
    ArrangementView();

private:
    ArrangementViewTopBar topBar;
    ArrangementViewTimeline timeline;

    void editChanged (Edit* newEdit) noexcept override;

    void resized() noexcept override;
    void mouseWheelMove (const MouseEvent&, const MouseWheelDetails&) override;
    void mouseMagnify (const MouseEvent&, float) override;

    void valueTreePropertyChanged (ValueTree&, const Identifier&) noexcept override;
    void valueTreeChildAdded (ValueTree&, ValueTree&) override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementView)
};
