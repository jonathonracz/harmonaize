/*
  ==============================================================================

    hmnz_ArrangementViewTrackLane.h
    Created: 9 Mar 2018 2:36:41pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ArrangementViewTimelineComponent.h"
#include "hmnz_HomogeneousValueTreeObjectArray.h"

class Track;
class Clip;
class ArrangementViewTrackLaneClip;

class ArrangementViewTrackLane  : public ArrangementViewTimelineComponent,
                                  public HomogeneousValueTreeObjectArray<Clip>::Listener,
                                  public ValueTree::Listener
{
public:
    ArrangementViewTrackLane (Track& track);
    ~ArrangementViewTrackLane();

private:
    Track& track;
    OwnedArray<ArrangementViewTrackLaneClip> clips;

    ArrangementViewTrackLaneClip* getChildForClip (Clip* clip);

    void resized() override;

    void objectAdded (Clip*, int, HomogeneousValueTreeObjectArray<Clip>*) override;
    void objectRemoved (Clip*, int, HomogeneousValueTreeObjectArray<Clip>*) override;

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&) override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementViewTrackLane)
};
