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
#include "hmnz_ArrangementViewPlayHead.h"
#include "hmnz_ArrangementViewHeaderResizer.h"

class Track;

class ArrangementView   : public Component,
                          public ValueTree::Listener
{
public:
    ArrangementView (Edit& edit);
    ~ArrangementView();

private:
    Edit& edit;

    ArrangementViewTopBar topBar;
    ArrangementViewTimelineGrid grid;
    ArrangementViewTrackList trackList;
    ArrangementViewPlayHead playHead;
    ArrangementViewHeaderResizer headerResizer;
    double verticalScrollAccumulator = 0.0;

    void resized() override;
    void mouseWheelMove (const MouseEvent&, const MouseWheelDetails&) override;
    void mouseMagnify (const MouseEvent&, float) override;

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&) override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementView)
};
