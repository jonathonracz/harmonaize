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

    Track* getRepresentedTrack() const { return track; }

private:
    WeakReference<Track> track;

    void paint (Graphics&) override;
    jcf::AdvancedLeakDetector leakDetector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementViewTrackHeader)
};
