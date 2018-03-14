/*
  ==============================================================================

    hmnz_ArrangementViewTimeline.cpp
    Created: 9 Mar 2018 3:57:50pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_ArrangementViewTimeline.h"

ArrangementViewTimeline::ArrangementViewTimeline()
{
    addAndMakeVisible (grid);
}

void ArrangementViewTimeline::editChanged (Edit* oldEdit)
{
    grid.setEdit (getEdit());
}

void ArrangementViewTimeline::resized()
{
    grid.setBounds (getLocalBounds());
}
