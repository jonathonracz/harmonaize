/*
  ==============================================================================

    hmnz_ArrangementViewTrackResizer.h
    Created: 17 Mar 2018 3:55:57am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class Track;

class ArrangementViewTrackResizer   : public Component
{
public:
    ArrangementViewTrackResizer (Track& track);

private:
    Track& track;
    int lastDragPos;

    void paint (Graphics& g) override;
    void mouseDown (const MouseEvent& event) override;
    void mouseDrag (const MouseEvent& event) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementViewTrackResizer)
};
