/*
  ==============================================================================

    hmnz_ArrangementViewTrackLane.h
    Created: 9 Mar 2018 2:36:41pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ArrangementViewComponent.h"
#include "hmnz_HomogeneousValueTreeObjectArray.h"

class Track;
class Clip;
class ArrangementViewTrackLaneClip;

class ArrangementViewTrackLane  : public ArrangementViewComponent,
                                  public HomogeneousValueTreeObjectArray<Clip>::Listener
{
public:
    ArrangementViewTrackLane (Track* track);
    ~ArrangementViewTrackLane();

    Track* getRepresentedTrack() const { return track; }

private:
    Track* track;
    OwnedArray<ArrangementViewTrackLaneClip> clips;

    ArrangementViewTrackLaneClip* getChildForClip (Clip* clip);

    void editChanged (Edit* oldEdit) override;

    void resized() override;

    void objectAdded (Clip*, int, HomogeneousValueTreeObjectArray<Clip>*) override;
    void objectRemoved (Clip*, int, HomogeneousValueTreeObjectArray<Clip>*) override;

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementViewTrackLane)
};
