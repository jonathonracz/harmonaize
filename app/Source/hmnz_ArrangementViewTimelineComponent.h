/*
  ==============================================================================

    hmnz_ArrangementViewTimelineComponent.h
    Created: 7 Mar 2018 1:17:01pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"

class Edit;

class ArrangementViewTimelineComponent  : public Component
{
public:
    ArrangementViewTimelineComponent (Edit& edit);

protected:
    Edit& edit;

    NormalisableRange<double> getBeatRemapper() const;
    int getXPosForBeat (double beat) const;
    double getBeatForXPos (int xPos) const;
    double getLinesPerBeatForMinimumLineSpacing (int minimumLineSpacing) const;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementViewTimelineComponent)
};
