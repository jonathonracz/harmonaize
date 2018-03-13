/*
  ==============================================================================

    hmnz_ArrangementViewTrackHeaderList.h
    Created: 7 Mar 2018 4:14:01pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ArrangementViewComponent.h"
#include "hmnz_ArrangementViewTrackHeader.h"
#include "hmnz_HomogeneousValueTreeObjectArray.h"

class ArrangementViewTrackHeaderList    : public ArrangementViewComponent,
                                          public HomogeneousValueTreeObjectArray<Track, CriticalSection>::Listener
{
public:
    ArrangementViewTrackHeaderList() = default;

private:
    OwnedArray<ArrangementViewTrackHeader> headers;

    void editChanged (Edit*) override;

    void resized() override;

    void objectAdded (Track*, int, HomogeneousValueTreeObjectArray<Track, CriticalSection>*) override;
    void objectRemoved (Track*, int, HomogeneousValueTreeObjectArray<Track, CriticalSection>*) override;
    void objectOrderChanged (Track*, int, int, HomogeneousValueTreeObjectArray<Track, CriticalSection>*) override;

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementViewTrackHeaderList)
};
