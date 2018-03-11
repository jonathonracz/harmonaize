/*
  ==============================================================================

    hmnz_ArrangementViewTrackLaneClip.h
    Created: 11 Mar 2018 12:03:40pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ArrangementViewComponent.h"

class Clip;

class ArrangementViewTrackLaneClip  : public ArrangementViewComponent
{
public:
    ArrangementViewTrackLaneClip (Clip* clip);
    ~ArrangementViewTrackLaneClip();

    Clip* getRepresentedClip() const noexcept { return clip; }

private:
    Clip* clip;

    void paint (Graphics&) noexcept override;

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementViewTrackLaneClip)
};
