/*
  ==============================================================================

    hmnz_ArrangementViewTrack.h
    Created: 14 Mar 2018 1:48:47pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ArrangementViewComponent.h"
#include "hmnz_ArrangementViewTrackHeader.h"
#include "hmnz_ArrangementViewTrackLane.h"

class ArrangementViewTrack  : public ArrangementViewComponent
{
public:
    ArrangementViewTrack (Track* representedTrack);

    Track* getRepresentedTrack() const { return track; }

private:
    Track* track;

    ArrangementViewTrackHeader header;
    ArrangementViewTrackLane lane;

    void editChanged (Edit*) override;

    void resized() override;

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
};
