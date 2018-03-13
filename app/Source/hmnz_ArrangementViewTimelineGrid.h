/*
  ==============================================================================

    hmnz_ArrangementViewTimelineGrid.h
    Created: 7 Mar 2018 1:09:19pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ArrangementViewComponent.h"

class ArrangementViewTimelineGrid   : public ArrangementViewComponent
{
public:
    ArrangementViewTimelineGrid() = default;

private:
    Rectangle<int> lastPaintedPlayHeadBounds;

    void resized() override;
    void paint (Graphics&) override;

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementViewTimelineGrid)
};
