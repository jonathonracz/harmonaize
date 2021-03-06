/*
  ==============================================================================

    hmnz_ArrangementViewTopBar.h
    Created: 7 Mar 2018 1:05:20pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ArrangementViewTimelineComponent.h"

class ArrangementViewTopBar : public ArrangementViewTimelineComponent,
                              public ValueTree::Listener
{
public:
    ArrangementViewTopBar (Edit& edit, CachedValue<double>& timeStart, CachedValue<double>& timeEnd);
    ~ArrangementViewTopBar();

private:
    Edit& edit;

    void paint (Graphics&) override;
    void mouseDown (const MouseEvent&) override;
    void mouseDrag (const MouseEvent&) override;

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&) override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementViewTopBar)
};
