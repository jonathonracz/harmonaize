/*
  ==============================================================================

    hmnz_ArrangementViewTimelineGrid.h
    Created: 7 Mar 2018 1:09:19pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ArrangementViewTimelineComponent.h"

class ArrangementViewTimelineGrid   : public ArrangementViewTimelineComponent,
                                      public ValueTree::Listener
{
public:
    ArrangementViewTimelineGrid (Edit& edit);
    ~ArrangementViewTimelineGrid();

private:
    Rectangle<int> lastPaintedPlayHeadBounds;

    void paint (Graphics&) override;

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&) override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementViewTimelineGrid)
};
