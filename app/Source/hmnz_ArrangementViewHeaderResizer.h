/*
  ==============================================================================

    hmnz_ArrangementViewHeaderResizer.h
    Created: 17 Mar 2018 4:13:32am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class Edit;

class ArrangementViewHeaderResizer   : public Component
{
public:
    ArrangementViewHeaderResizer (Edit& edit);

private:
    Edit& edit;
    int lastDragPos;

    void paint (Graphics& g) override;
    void mouseDown(const MouseEvent& event) override;
    void mouseDrag (const MouseEvent& event) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementViewHeaderResizer)
};
