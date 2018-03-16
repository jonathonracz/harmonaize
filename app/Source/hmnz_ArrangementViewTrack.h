/*
  ==============================================================================

    hmnz_ArrangementViewTrack.h
    Created: 14 Mar 2018 1:48:47pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ArrangementViewTrackHeader.h"
#include "hmnz_ArrangementViewTrackLane.h"

class ArrangementViewTrack  : public Component,
                              public ValueTree::Listener
{
public:
    ArrangementViewTrack (Track& representedTrack);
    ~ArrangementViewTrack();

    Track& getRepresentedTrack() const { return track; }

private:
    Track& track;

    ArrangementViewTrackHeader header;
    ArrangementViewTrackLane lane;

    void resized() override;

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&) override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}
};
