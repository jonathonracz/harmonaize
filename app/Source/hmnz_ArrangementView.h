/*
  ==============================================================================

    hmnz_ArrangementView.h
    Created: 4 Mar 2018 11:27:08pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class Edit;

class ArrangementView   : public Component,
                          public ValueTree::Listener
{
public:
    ArrangementView() {}

    void setEdit (Edit* edit);

private:
    WeakReference<Edit> edit;

    void resized() noexcept override;
    void paint (Graphics&) noexcept override;

    void valueTreePropertyChanged (ValueTree&, const Identifier&) noexcept override;
    void valueTreeChildAdded (ValueTree&, ValueTree&) override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementView)
};
