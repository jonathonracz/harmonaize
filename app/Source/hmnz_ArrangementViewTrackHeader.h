/*
  ==============================================================================

    hmnz_ArrangementViewTrackHeader.h
    Created: 7 Mar 2018 4:47:06pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ArrangementViewComponent.h"

class Track;

class ArrangementViewTrackHeader  : public ArrangementViewComponent
{
public:
    ArrangementViewTrackHeader (Track* track);

    Track* getRepresentedTrack() const noexcept { return track; }

private:
    Track* track;

    void editChanged () noexcept override;

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override {}
    void valueTreeChildAdded (ValueTree&, ValueTree&) override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementViewTrackHeader)
};
