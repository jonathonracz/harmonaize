/*
  ==============================================================================

    hmnz_PreferencesView.cpp
    Created: 18 Feb 2018 11:29:28pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_PreferencesView.h"
#include "hmnz_Application.h"

PreferencesView::PreferencesView()
    : selectorComponent (HarmonaizeApplication::getDeviceManager(), 0, 2, 0, 2, true, false, true, true)
{
    setOpaque (true);
    addAndMakeVisible (selectorComponent);
}

void PreferencesView::resized()
{
    FlexBox mainBox;

    mainBox.items.add (FlexItem (selectorComponent).withFlex (1.0f));

    mainBox.performLayout (getLocalBounds());
}

void PreferencesView::paint (Graphics& g)
{
    g.fillAll (Colours::grey);
}

void PreferencesView::userTriedToCloseWindow()
{
    this->setVisible (false);
}
