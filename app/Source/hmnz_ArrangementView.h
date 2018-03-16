/*
  ==============================================================================

    hmnz_ArrangementView.h
    Created: 4 Mar 2018 11:27:08pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ArrangementViewTopBar.h"
#include "hmnz_ArrangementViewTimelineGrid.h"
#include "hmnz_ArrangementViewTrackList.h"

class Track;

class ArrangementView   : public Component
{
public:
    ArrangementView (Edit& edit);

private:
    Edit& edit;

    ArrangementViewTopBar topBar;
    ArrangementViewTimelineGrid grid;
    ArrangementViewTrackList trackList;
    double verticalScrollAccumulator = 0.0;

    void resized() override;
    void mouseWheelMove (const MouseEvent&, const MouseWheelDetails&) override;
    void mouseMagnify (const MouseEvent&, float) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementView)
};
