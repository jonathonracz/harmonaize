/*
  ==============================================================================

    hmnz_HintView.h
    Created: 12 Apr 2018 12:58:52pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class HintView  : public Component,
                  public SettableTooltipClient
{
public:
    HintView();

    void setComponentToWatch (Component* _componentToWatch);

    void mouseEnter (const MouseEvent& event) override;
    void mouseExit (const MouseEvent& event) override;
    void resized() override;

private:
    void setContentForComponent (Component* component);

    Label name;
    Label description;
    WeakReference<Component> componentToWatch;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HintView)
};
