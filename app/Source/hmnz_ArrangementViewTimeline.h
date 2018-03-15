/*
  ==============================================================================

    hmnz_ArrangementViewTimeline.h
    Created: 9 Mar 2018 3:57:50pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ArrangementViewTimelineGrid.h"

class ArrangementViewTimeline   : public ArrangementViewComponent
{
public:
    ArrangementViewTimeline();

private:
    ArrangementViewTimelineGrid grid;

    void editChanged (Edit* oldEdit) override;

    void resized() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementViewTimeline)
};