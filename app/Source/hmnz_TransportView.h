/*
  ==============================================================================

    hmnz_TransportView.h
    Created: 12 Feb 2018 11:38:59am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class Transport;

class TransportView : public Component,
                      public ValueTree::Listener,
                      public Button::Listener,
                      public Label::Listener,
                      public ComboBox::Listener
{
public:
    TransportView();

    void setTransport (Transport* transport);

private:
    WeakReference<Transport> transport;

    TextButton goToBeginningButton;
    TextButton stopPlayButton;
    TextButton recordButton;

    Label timeLabel;
    Label beatLabel;
    Label tempoLabel;
    Label timeSignatureLabel;
    
    ComboBox keySignatureComboBox;

    Label timeText;
    Label beatText;
    Label tempoText;
    Label timeSignatureText;
    Label keySignatureText;

    void resized() override;
    void paint (Graphics&) override;

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&) override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

    void buttonClicked (Button*) override;
    void buttonStateChanged (Button*) override;
    
    void comboBoxChanged (ComboBox*) override;

    void labelTextChanged (Label*) override {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TransportView)
};
