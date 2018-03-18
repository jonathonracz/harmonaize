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
    ArrangementViewTimelineComponent (CachedValue<double>& timeStart, CachedValue<double>& timeEnd);

    NormalisableRange<double> getBeatRemapper() const;
    int getXPosForBeat (double beat) const;
    double getBeatForXPos (int xPos) const;
    double getLinesPerBeatForMinimumLineSpacing (int minimumLineSpacing) const;

protected:
    CachedValue<double>& timeStart;
    CachedValue<double>& timeEnd;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementViewTimelineComponent)
};
