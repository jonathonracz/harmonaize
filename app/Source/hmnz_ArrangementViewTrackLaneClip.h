/*
  ==============================================================================

    hmnz_ArrangementViewTrackLaneClip.h
    Created: 11 Mar 2018 12:03:40pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ArrangementViewTimelineComponent.h"

class Clip;

class ArrangementViewTrackLaneClip  : public ArrangementViewTimelineComponent,
                                      public ValueTree::Listener
{
public:
    ArrangementViewTrackLaneClip (Clip& clip);
    ~ArrangementViewTrackLaneClip();

    Clip& getRepresentedClip() const { return clip; }

private:
    Clip& clip;

    void paint (Graphics&) override;

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&) override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementViewTrackLaneClip)
};
