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

TransportView::TransportView()
{
    goToBeginningButton.addListener (this);
    stopPlayButton.addListener (this);
    recordButton.addListener (this);

    timeLabel.addListener (this);
    beatLabel.addListener (this);
    tempoLabel.addListener (this);
    timeSignatureLabel.addListener (this);
    keySignatureLabel.addListener (this);

    goToBeginningButton.setButtonText (translate ("Go to beginning"));
    stopPlayButton.setButtonText (translate ("Play"));
    recordButton.setButtonText (translate ("Record"));

    timeText.setText (translate ("Seconds"));
    beatText.setText (translate ("Beat"));
    tempoText.setText (translate ("Tempo"));
    timeSignatureText.setText (translate ("Time"));
    keySignatureText.setText (translate ("Key"));

    stopPlayButton.setClickingTogglesState (true);
    recordButton.setClickingTogglesState (true);

    addAndMakeVisible (goToBeginningButton);
    addAndMakeVisible (stopPlayButton);
    addAndMakeVisible (recordButton);

    addAndMakeVisible (timeLabel);
    addAndMakeVisible (beatLabel);
    addAndMakeVisible (tempoLabel);
    addAndMakeVisible (timeSignatureLabel);
    addAndMakeVisible (keySignatureLabel);

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
        //keySignatureLabel.setText (String (transport->playPositionTime), NotificationType::dontSendNotification);
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
    //text.items.add (FlexItem (keySignatureText).withFlex (1.0f));

    FlexBox labels;
    labels.flexDirection = FlexBox::Direction::row;
    labels.items.add (FlexItem (timeLabel).withFlex (1.0f));
    labels.items.add (FlexItem (beatLabel).withFlex (1.0f));
    labels.items.add (FlexItem (tempoLabel).withFlex (1.0f));
    //labels.items.add (FlexItem (timeSignatureLabel).withFlex (1.0f));
    //labels.items.add (FlexItem (keySignatureLabel).withFlex (1.0f));

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
