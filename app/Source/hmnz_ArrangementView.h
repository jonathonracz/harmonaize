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
#include "hmnz_ArrangementViewTrackList.h"

class Track;

class ArrangementView   : public ArrangementViewComponent
{
public:
    ArrangementView();

private:
    ArrangementViewTopBar topBar;
    ArrangementViewTimeline timeline;
    ArrangementViewTrackList trackList;
    double verticalScrollAccumulator = 0.0;

    void editChanged (Edit*) override;

    void resized() override;
    void mouseWheelMove (const MouseEvent&, const MouseWheelDetails&) override;
    void mouseMagnify (const MouseEvent&, float) override;

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementView)
};
