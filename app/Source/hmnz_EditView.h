/*
  ==============================================================================

    hmnz_EditView.h
    Created: 30 Jan 2018 1:12:54pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_Edit.h"
#include "hmnz_HintView.h"
#include "hmnz_TransportView.h"
#include "hmnz_ArrangementView.h"
#include "hmnz_CustomLookAndFeel.h"

class EditView  : public Component,
                  public ValueTree::Listener,
                  public SettableTooltipClient
{
public:
    EditView (Edit& edit);
    ~EditView();

    CustomLookAndFeel* customLookAndFeel;

private:
    Edit& edit;

    HintView hintView;
    TransportView transportView;
    ArrangementView arrangementView;

    class MidiKeyboardComponentWithHint : public MidiKeyboardComponent,
                                          public SettableTooltipClient
    {
        using MidiKeyboardComponent::MidiKeyboardComponent;
    };

    std::unique_ptr<MidiKeyboardComponentWithHint> keyboard;

    void resized() override;
    void paint (Graphics&) override;

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override {}
    void valueTreeChildAdded (ValueTree&, ValueTree&) override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditView)
};
