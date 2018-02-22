/*
  ==============================================================================

    hmnz_TransportView.cpp
    Created: 12 Feb 2018 11:50:03am
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_TransportView.h"
#include "hmnz_Transport.h"
#include "hmnz_Edit.h"
#include "hmnz_KeySignature.h"

TransportView::TransportView()
{
    goToBeginningButton.addListener (this);
    stopPlayButton.addListener (this);
    recordButton.addListener (this);
    clearButton.addListener(this);
    generateAccompanimentButton.addListener(this);

    timeLabel.addListener (this);
    beatLabel.addListener (this);
    tempoSlider.addListener (this);
    timeSignatureLabel.addListener (this);
    keySignatureComboBox.addListener (this);

    goToBeginningButton.setButtonText (translate ("Go to beginning"));
    stopPlayButton.setButtonText (translate ("Play"));
    recordButton.setButtonText (translate ("Record"));
    clearButton.setButtonText(translate("Clear"));
    generateAccompanimentButton.setButtonText(translate("Generate\nAccompaniment"));
    
    recordButton.setColour(TextButton::ColourIds::buttonOnColourId, Colours::red);

    timeText.setText (translate ("Seconds"), NotificationType::dontSendNotification);
    beatText.setText (translate ("Beat"), NotificationType::dontSendNotification);
    tempoText.setText (translate ("Tempo"), NotificationType::dontSendNotification);
    timeText.setEditable(false);
    beatText.setEditable(false);
    tempoText.setEditable(false);
//    timeSignatureText.setText (translate ("Time"));
    keySignatureText.setText (translate ("Key"), NotificationType::dontSendNotification);

    stopPlayButton.setClickingTogglesState (true);
    recordButton.setClickingTogglesState (true);

    addAndMakeVisible (goToBeginningButton);
    addAndMakeVisible (stopPlayButton);
    addAndMakeVisible (recordButton);
    addAndMakeVisible(clearButton);
    addAndMakeVisible(generateAccompanimentButton);

    addAndMakeVisible (timeLabel);
    addAndMakeVisible (beatLabel);
    addAndMakeVisible (tempoSlider);
    addAndMakeVisible (timeSignatureLabel);
    addAndMakeVisible (keySignatureComboBox);

    addAndMakeVisible (timeText);
    addAndMakeVisible (beatText);
    addAndMakeVisible (tempoText);
    addAndMakeVisible (timeSignatureText);
    addAndMakeVisible (keySignatureText);
}

void TransportView::setTransport (Transport* _transport)
{
    if (transport)
        transport->getState().removeListener (this);

    transport = _transport;

    if (transport)
    {
        transport->getState().addListener (this);
        timeLabel.setText (String (transport->playHeadTime), NotificationType::dontSendNotification);
        beatLabel.setText (String (transport->playHeadBeat), NotificationType::dontSendNotification);
//        tempoSlider = Slider(Slider::SliderStyle::LinearHorizontal, Slider::TextEntryBoxPosition::TextBoxBelow);
        tempoSlider.setColour(Slider::ColourIds::textBoxOutlineColourId, Colours::transparentWhite);
        tempoSlider.setTextBoxStyle (Slider::TextBoxBelow, 0, 80, 25);
        tempoSlider.setRange (1, 200);
        tempoSlider.setTextValueSuffix (" BPM");
//        tempoLabel.setText (String (transport->playHeadTempo), NotificationType::dontSendNotification);
        timeSignatureLabel.setText (String (transport->playHeadTimeSigNumerator) + "/" + String (transport->playHeadTimeSigDenominator), NotificationType::dontSendNotification);
        //timeSignatureLabel.setText (String (transport->playPositionTime), NotificationType::dontSendNotification);
        StringArray items = KeySignature::getKeyDescriptions();
        keySignatureComboBox.addItemList(items, 1);
    }
}

void TransportView::resized()
{
    FlexBox mainBox;
    mainBox.flexDirection = FlexBox::Direction::column;

    FlexBox text;
    text.flexDirection = FlexBox::Direction::row;
    FlexItem timeT = FlexItem (timeText).withFlex (1.0f);
    FlexItem beatT = FlexItem (beatText).withFlex (1.0f);
    FlexItem tempoT = FlexItem (tempoText).withFlex (1.0f);
    FlexItem keySigT = FlexItem (keySignatureText).withFlex (1.0f);
    timeT.height = 50;
    beatT.height = 50;
    tempoT.height = 50;
    keySigT.height = 50;
    text.items.add (timeT);
    text.items.add (beatT);
    text.items.add (tempoT);
    //text.items.add (FlexItem (timeSignatureText).withFlex (1.0f));
    text.items.add (keySigT);

    FlexBox labels;
    labels.flexDirection = FlexBox::Direction::row;
    FlexItem timeL = FlexItem (timeLabel).withFlex (1.0f);
    FlexItem beatL = FlexItem (beatLabel).withFlex (1.0f);
    FlexItem tempoL = FlexItem (tempoSlider).withFlex (1.0f);
    FlexItem keySigL = FlexItem (keySignatureComboBox).withFlex (1.0f);
    timeL.height = 50;
    beatL.height = 50;
    tempoL.height = 50;
    keySigL.height = 50;
    labels.items.add (timeL);
    labels.items.add (beatL);
    labels.items.add (tempoL);
    //labels.items.add (FlexItem (timeSignatureLabel).withFlex (1.0f));
    labels.items.add (keySigL);

    FlexBox buttons;
    buttons.flexDirection = FlexBox::Direction::row;
    buttons.items.add (FlexItem (goToBeginningButton).withFlex (1.0f));
    buttons.items.add (FlexItem (stopPlayButton).withFlex (1.0f));
    buttons.items.add (FlexItem (recordButton).withFlex (1.0f));
    buttons.items.add (FlexItem (clearButton).withFlex (1.0f));
    buttons.items.add (FlexItem (generateAccompanimentButton).withFlex (1.0f));

    FlexItem textFlex = FlexItem (text).withFlex (0.0f);
    FlexItem labelsFlex = FlexItem (labels).withFlex (0.0f);
    FlexItem buttonsFlex = FlexItem (buttons).withFlex (1.0f);
    textFlex.height = 50;
    labelsFlex.height = 50;
    labelsFlex.margin = 10;
//    textFlex.alignSelf = FlexItem::AlignSelf::flexStart;
    mainBox.items.add (textFlex);
    mainBox.items.add (labelsFlex);
    mainBox.items.add (buttonsFlex);
    mainBox.performLayout (getLocalBounds());
}

void TransportView::paint (Graphics& g)
{
    g.fillAll (Colours::grey);
}

void TransportView::valueTreePropertyChanged (ValueTree& treeChanged, const Identifier& property)
{
    jassert (transport && treeChanged == transport->getState());
    if (property == transport->playState.getPropertyID())
    {
        if (int (treeChanged[property]) == Transport::State::playing)
            stopPlayButton.setToggleState (true, NotificationType::dontSendNotification);
        else if (int (treeChanged[property]) == Transport::State::stopped)
            stopPlayButton.setToggleState (false, NotificationType::dontSendNotification);
    }
    else if (property == transport->playHeadTime.getPropertyID())
    {
        timeLabel.setText (String (transport->playHeadTime), NotificationType::dontSendNotification);
    }
    else if (property == transport->playHeadBeat.getPropertyID())
    {
        beatLabel.setText (String (transport->playHeadBeat), NotificationType::dontSendNotification);
    }
    else if (property == transport->playHeadKeySigNumSharpsOrFlats.getPropertyID())
    {
        bool minor = transport->playHeadKeySigIsMinor.get();
        int numSharpsOrFlats = transport->playHeadKeySigNumSharpsOrFlats.get();
        String desc = KeySignature::createKeyDescription(numSharpsOrFlats, minor);
        for (int i = 0; i < keySignatureComboBox.getNumItems(); i++) {
            String val = keySignatureComboBox.getItemText(i);
            if (val == desc) {
                keySignatureComboBox.setSelectedItemIndex(i, NotificationType::dontSendNotification);
            }
        }
    }
    else if (property == transport->playHeadKeySigIsMinor.getPropertyID())
    {
        bool minor = transport->playHeadKeySigIsMinor.get();
        int numSharpsOrFlats = transport->playHeadKeySigNumSharpsOrFlats.get();
        String desc = KeySignature::createKeyDescription(numSharpsOrFlats, minor);
        for (int i = 0; i < keySignatureComboBox.getNumItems(); i++) {
            String val = keySignatureComboBox.getItemText(i);
            if (val == desc) {
                keySignatureComboBox.setSelectedItemIndex(i, NotificationType::dontSendNotification);
            }
        }
    }
    else if (property == transport->playHeadTempo.getPropertyID())
    {
        tempoSlider.setValue(transport->playHeadTempo, NotificationType::dontSendNotification);
    }
}

void TransportView::buttonClicked (Button* button)
{
    if (button == &goToBeginningButton)
    {
        if (transport)
            transport->playHeadTime = 0.0f;
    }
    else if (button == &stopPlayButton)
    {
        if (transport)
            transport->transportPlay (button->getToggleState());
    }
    else if (button == &recordButton)
    {
        if (transport)
        {
            transport->transportRecord (button->getToggleState());
            transport->transportPlay (button->getToggleState());
        }
    }
    else if (button == &clearButton)
    {
        if (transport)
        {
            transport->playHeadTime = 0.0f;
            // TODO: Erase current melody
        }
    }
    else if (button == &generateAccompanimentButton)
    {
        // TODO: Add functionality to generate accompaniment on current melody
    }
}

void TransportView::buttonStateChanged (Button* button)
{
    if (button == &stopPlayButton)
    {
        if (button->getToggleState())
            button->setButtonText (translate ("Stop"));
        else
            button->setButtonText (translate ("Play"));
    }
}

void TransportView::comboBoxChanged (ComboBox* comboBox)
{
    String val = comboBox->getText();
    std::pair<int, bool> key = KeySignature::createRepresentationFromDescription(val);
    transport->playHeadKeySigNumSharpsOrFlats = key.first;
    transport->playHeadKeySigIsMinor = key.second;
}

void TransportView::sliderValueChanged (Slider* slider)
{
    int val = slider->getValue();
    slider->setValue(val);
    transport->playHeadTempo = val;
}
