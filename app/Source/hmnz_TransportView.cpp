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

    timeLabel.addListener (this);
    beatLabel.addListener (this);
    tempoLabel.addListener (this);
    timeSignatureLabel.addListener (this);
    keySignatureComboBox.addListener (this);

    goToBeginningButton.setButtonText (translate ("Go to beginning"));
    stopPlayButton.setButtonText (translate ("Play"));
    recordButton.setButtonText (translate ("Record"));

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

    addAndMakeVisible (timeLabel);
    addAndMakeVisible (beatLabel);
    addAndMakeVisible (tempoLabel);
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
        tempoLabel.setText (String (transport->playHeadTempo), NotificationType::dontSendNotification);
        timeSignatureLabel.setText (String (transport->playHeadTimeSigNumerator) + "/" + String (transport->playHeadTimeSigDenominator), NotificationType::dontSendNotification);
        //timeSignatureLabel.setText (String (transport->playPositionTime), NotificationType::dontSendNotification);
        StringArray items = KeySignature::getKeyDescriptions();
        keySignatureComboBox.addItemList(items, 1);
//        keySignatureLabel.setText (String (transport->playPositionTime), NotificationType::dontSendNotification);
    }
}

void TransportView::resized()
{
    FlexBox mainBox;
    mainBox.flexDirection = FlexBox::Direction::column;

    FlexBox text;
    text.flexDirection = FlexBox::Direction::row;
    text.items.add (FlexItem (timeText).withFlex (1.0f));
    text.items.add (FlexItem (beatText).withFlex (1.0f));
    text.items.add (FlexItem (tempoText).withFlex (1.0f));
    //text.items.add (FlexItem (timeSignatureText).withFlex (1.0f));
    text.items.add (FlexItem (keySignatureText).withFlex (1.0f));

    FlexBox labels;
    labels.flexDirection = FlexBox::Direction::row;
    labels.items.add (FlexItem (timeLabel).withFlex (1.0f));
    labels.items.add (FlexItem (beatLabel).withFlex (1.0f));
    labels.items.add (FlexItem (tempoLabel).withFlex (1.0f));
    //labels.items.add (FlexItem (timeSignatureLabel).withFlex (1.0f));
    labels.items.add (FlexItem (keySignatureComboBox).withFlex (1.0f));

    FlexBox buttons;
    buttons.flexDirection = FlexBox::Direction::row;
    buttons.items.add (FlexItem (goToBeginningButton).withFlex (1.0f));
    buttons.items.add (FlexItem (stopPlayButton).withFlex (1.0f));
    buttons.items.add (FlexItem (recordButton).withFlex (1.0f));

    mainBox.items.add (FlexItem (text).withFlex (1.0f));
    mainBox.items.add (FlexItem (labels).withFlex (1.0f));
    mainBox.items.add (FlexItem (buttons).withFlex (1.0f));
    mainBox.performLayout (getLocalBounds());
}

void TransportView::paint (Graphics& g)
{
    g.fillAll (Colours::blue);
}

void TransportView::valueTreePropertyChanged (ValueTree& treeChanged, const Identifier& property)
{
    jassert (transport && treeChanged == transport->getState());
    if (property == transport->playState.getPropertyID())
    {
        if (int (treeChanged[property]) == Transport::State::playing)
            stopPlayButton.setToggleState (true, NotificationType::sendNotification);
        else if (int (treeChanged[property]) == Transport::State::stopped)
            stopPlayButton.setToggleState (false, NotificationType::sendNotification);
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
                //                int select = keySignatureComboBox.getItemId(i);
                keySignatureComboBox.setSelectedItemIndex(i, dontSendNotification);
                //                keySignatureComboBox.setSelectedId(select, dontSendNotification);
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
//                int select = keySignatureComboBox.getItemId(i);
                keySignatureComboBox.setSelectedItemIndex(i, dontSendNotification);
//                keySignatureComboBox.setSelectedId(select, dontSendNotification);
            }
        }
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
            transport->transportRecord (button->getToggleState());
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
//    int id = comboBox->getSelectedId();
    String val = comboBox->getText();
    std::pair<int, bool> key = KeySignature::createRepresentationFromDescription(val);
    transport->playHeadKeySigNumSharpsOrFlats = key.first;
    transport->playHeadKeySigIsMinor = key.second;
}
