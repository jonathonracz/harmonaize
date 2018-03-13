/*
  ==============================================================================

    hmnz_ArrangementViewTrackLaneList.h
    Created: 9 Mar 2018 2:36:58pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ArrangementViewComponent.h"
#include "hmnz_ArrangementViewTrackLane.h"
#include "hmnz_HomogeneousValueTreeObjectArray.h"

class ArrangementViewTrackLaneList  : public ArrangementViewComponent,
                                      public HomogeneousValueTreeObjectArray<Track, CriticalSection>::Listener
{
public:
    ArrangementViewTrackLaneList() = default;

private:
    OwnedArray<ArrangementViewTrackLane> lanes;

    void editChanged (Edit*) override;

    void resized() override;

    void objectAdded (Track*, int, HomogeneousValueTreeObjectArray<Track, CriticalSection>*) override;
    void objectRemoved (Track*, int, HomogeneousValueTreeObjectArray<Track, CriticalSection>*) override;
    void objectOrderChanged (Track*, int, int, HomogeneousValueTreeObjectArray<Track, CriticalSection>*) override;

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementViewTrackLaneList)
};
