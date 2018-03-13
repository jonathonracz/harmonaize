/*
  ==============================================================================

    hmnz_PreferencesView.h
    Created: 18 Feb 2018 11:15:29pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class PreferencesView   : public Component
{
public:
    PreferencesView();

private:
    AudioDeviceSelectorComponent selectorComponent;

    void resized() override;
    void paint (Graphics&) override;
    void userTriedToCloseWindow() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PreferencesView);
};
