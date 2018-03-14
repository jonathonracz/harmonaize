/*
  ==============================================================================

    hmnz_ArrangementViewTrackList.h
    Created: 14 Mar 2018 1:47:29pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ArrangementViewComponent.h"
#include "hmnz_HomogeneousValueTreeObjectArray.h"
#include "hmnz_ArrangementViewTrack.h"

class Edit;
class Track;

class ArrangementViewTrackList  : public ArrangementViewComponent,
                                  public HomogeneousValueTreeObjectArray<Track, CriticalSection>::Listener
{
public:
    ArrangementViewTrackList() = default;

private:
    OwnedArray<ArrangementViewTrack> tracks;

    void editChanged (Edit*) override;

    void resized() override;

    void objectAdded (Track*, int, HomogeneousValueTreeObjectArray<Track, CriticalSection>*) override;
    void objectRemoved (Track*, int, HomogeneousValueTreeObjectArray<Track, CriticalSection>*) override;
    void objectOrderChanged (Track*, int, int, HomogeneousValueTreeObjectArray<Track, CriticalSection>*) override;

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementViewTrackList)
};
