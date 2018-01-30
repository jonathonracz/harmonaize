/*
  ==============================================================================

    hmnz_EditView.h
    Created: 30 Jan 2018 1:12:54pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_Identifiers.h"
#include "hmnz_ValueTreeObject.h"
#include "hmnz_Edit.h"

class EditView  : public Component,
                  public ValueTree::Listener
{
public:
    EditView();
    ~EditView();

    void setEdit (Edit* edit);

private:
    WeakReference<Edit> currentEdit;

    void resized() override;
    void paint (Graphics&) override;

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override {}
    void valueTreeChildAdded (ValueTree&, ValueTree&) override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditView)
};
