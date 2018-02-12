/*
  ==============================================================================

    hmnz_TransportView.cpp
    Created: 12 Feb 2018 11:50:03am
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_TransportView.h"
#include "hmnz_Transport.h"

TransportView::TransportView()
{
    goToBeginningButton.addListener (this);
    stopPlayButton.addListener (this);
    recordButton.addListener (this);

    timeLabel.addListener (this);
    beatLabel.addListener (this);
    tempoLabel.addListener (this);
    timeSignatureLabel.addListener (this);

    goToBeginningButton.setButtonText (translate ("Go to beginning"));
    recordButton.setButtonText (translate ("Record"));

    stopPlayButton.setClickingTogglesState (true);
    recordButton.setClickingTogglesState (true);

    addAndMakeVisible (goToBeginningButton);
    addAndMakeVisible (stopPlayButton);
    addAndMakeVisible (recordButton);
    addAndMakeVisible (timeLabel);
    addAndMakeVisible (beatLabel);
    addAndMakeVisible (tempoLabel);
    addAndMakeVisible (timeSignatureLabel);
}

void TransportView::setTransport (Transport* _transport)
{
    if (transport)
        transport->getState().removeListener (this);

    transport = _transport;

    if (transport)
        transport->getState().addListener (this);
}

void TransportView::resized()
{
    FlexBox mainBox;
    mainBox.flexDirection = FlexBox::Direction::row;

    mainBox.items.add (FlexItem (goToBeginningButton).withFlex (1.0f));
    mainBox.items.add (FlexItem (stopPlayButton).withFlex (1.0f));
    mainBox.items.add (FlexItem (recordButton).withFlex (1.0f));

    mainBox.performLayout (getLocalBounds());
}

void TransportView::paint (Graphics& g)
{
    g.fillAll (Colours::blue);
}

void TransportView::valueTreePropertyChanged (ValueTree& treeChanged, const Identifier& property)
{
    jassert (transport && treeChanged == transport->getState());
    if (property == IDs::TransportProps::PlayState)
    {
        if (int (treeChanged[property]) == Transport::State::playing)
            stopPlayButton.setToggleState (true, NotificationType::sendNotification);
        else if (int (treeChanged[property]) == Transport::State::stopped)
            stopPlayButton.setToggleState (false, NotificationType::sendNotification);
    }
}

void TransportView::buttonClicked (Button* button)
{
    if (button == &goToBeginningButton)
    {
        if (transport)
            transport->playPositionTime = 0.0f;
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
