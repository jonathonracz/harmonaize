/*
  ==============================================================================

    hmnz_ArrangementViewTrackList.h
    Created: 14 Mar 2018 1:47:29pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_HomogeneousValueTreeObjectArray.h"
#include "hmnz_ArrangementViewTrack.h"

class Edit;
class Track;

class ArrangementViewTrackList  : public Component,
                                  public HomogeneousValueTreeObjectArray<Track, CriticalSection>::Listener,
                                  public ValueTree::Listener
{
public:
    ArrangementViewTrackList (Edit& edit);
    ~ArrangementViewTrackList();

private:
    Edit& edit;

    OwnedArray<ArrangementViewTrack> tracks;

    void resized() override;

    void objectAdded (Track*, int, HomogeneousValueTreeObjectArray<Track, CriticalSection>*) override;
    void objectRemoved (Track*, int, HomogeneousValueTreeObjectArray<Track, CriticalSection>*) override;
    void objectOrderChanged (Track*, int, int, HomogeneousValueTreeObjectArray<Track, CriticalSection>*) override;

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&) override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementViewTrackList)
};
