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
#include "../../harmonaize/midiinterchange/hmnz_MidiInterchange.h"

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
    clearButton.setButtonText (translate ("Clear"));
    generateAccompanimentButton.setButtonText (translate ("Generate\nAccompaniment"));
    
    recordButton.setColour (TextButton::ColourIds::buttonOnColourId, Colours::red);

    timeText.setText (translate ("Seconds"), NotificationType::dontSendNotification);
    beatText.setText (translate ("Beat"), NotificationType::dontSendNotification);
    tempoText.setText (translate ("Tempo"), NotificationType::dontSendNotification);
    timeText.setEditable (false);
    beatText.setEditable (false);
    tempoText.setEditable (false);
//    timeSignatureText.setText (translate ("Time"));
    keySignatureText.setText (translate ("Key"), NotificationType::dontSendNotification);

    stopPlayButton.setClickingTogglesState (true);
    recordButton.setClickingTogglesState (true);

    addAndMakeVisible (goToBeginningButton);
    addAndMakeVisible (stopPlayButton);
    addAndMakeVisible (recordButton);
    addAndMakeVisible (clearButton);
    addAndMakeVisible (generateAccompanimentButton);

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
        tempoSlider.setColour (Slider::ColourIds::textBoxOutlineColourId, Colours::transparentWhite);
        tempoSlider.setTextBoxStyle (Slider::TextBoxBelow, 0, 80, 25);
        tempoSlider.setRange (1, 200);
        tempoSlider.setTextValueSuffix (" BPM");
        timeSignatureLabel.setText (String (transport->playHeadTimeSigNumerator) + "/" + String (transport->playHeadTimeSigDenominator), NotificationType::dontSendNotification);
        //timeSignatureLabel.setText (String (transport->playPositionTime), NotificationType::dontSendNotification);
        StringArray items = KeySignature::getKeyDescriptions();
        keySignatureComboBox.addItemList (items, 1);
    }
}

void TransportView::resized()
{
    FlexBox mainBox;
    mainBox.flexDirection = FlexBox::Direction::column;

    FlexBox text;
    text.flexDirection = FlexBox::Direction::row;
    timeText.setJustificationType(Justification::centred);
    beatText.setJustificationType(Justification::centred);
    tempoText.setJustificationType(Justification::centred);
    keySignatureText.setJustificationType(Justification::centred);
    Font font = Font (20);
    timeText.setFont (font);
    beatText.setFont (font);
    tempoText.setFont (font);
    keySignatureText.setFont (font);
    FlexItem timeT = FlexItem (timeText).withFlex (0.5f);
    FlexItem beatT = FlexItem (beatText).withFlex (0.5f);
    FlexItem tempoT = FlexItem (tempoText).withFlex (0.5f);
    FlexItem keySigT = FlexItem (keySignatureText).withFlex (0.5f);
    text.items.add (FlexItem().withFlex (1.0f));
    text.items.add (timeT);
    text.items.add (beatT);
    text.items.add (tempoT);
    //text.items.add (FlexItem (timeSignatureText).withFlex (1.0f));
    text.items.add (keySigT);
    text.items.add (FlexItem().withFlex (1.0f));

    FlexBox labels;
    labels.flexDirection = FlexBox::Direction::row;
    timeLabel.setJustificationType(Justification::centred);
    beatLabel.setJustificationType(Justification::centred);
    timeLabel.setFont (font);
    beatLabel.setFont (font); 
    FlexItem timeL = FlexItem (timeLabel).withFlex (0.5f);
    FlexItem beatL = FlexItem (beatLabel).withFlex (0.5f);
    FlexItem tempoL = FlexItem (tempoSlider).withFlex (0.5f);
    FlexItem keySigL = FlexItem (keySignatureComboBox).withFlex (0.5f);
    labels.items.add (FlexItem().withFlex (1.0f));
    labels.items.add (timeL);
    labels.items.add (beatL);
    labels.items.add (tempoL);
    //labels.items.add (FlexItem (timeSignatureLabel).withFlex (1.0f));
    labels.items.add (keySigL);
    labels.items.add (FlexItem().withFlex (1.0f));

    FlexBox buttons;
    buttons.flexDirection = FlexBox::Direction::row;
    FlexItem goToBeginningB = FlexItem (goToBeginningButton).withFlex (1.0f);
    FlexItem stopPlayB = FlexItem (stopPlayButton).withFlex (1.0f);
    FlexItem recordB = FlexItem (recordButton).withFlex (1.0f);
    FlexItem clearB = FlexItem (clearButton).withFlex (1.0f);
    FlexItem generateAccompanimentB = FlexItem (generateAccompanimentButton).withFlex (1.0f);
    goToBeginningB.margin = 5;
    stopPlayB.margin = 5;
    recordB.margin = 5;
    clearB.margin = 5;
    generateAccompanimentB.margin = 5;
    buttons.items.add (goToBeginningB);
    buttons.items.add (stopPlayB);
    buttons.items.add (recordB);
    buttons.items.add (clearB);
    buttons.items.add (generateAccompanimentB);

    FlexItem textFlex = FlexItem (text).withFlex (0.3f);
    FlexItem labelsFlex = FlexItem (labels).withFlex (0.3f);
    FlexItem buttonsFlex = FlexItem (buttons).withFlex (1.0f);
    labelsFlex.margin = 10;
    buttonsFlex.margin = 10;
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
        buttonStateChanged (&stopPlayButton);
    }
    else if (property == transport->recordEnabled.getPropertyID())
    {
        recordButton.setToggleState (bool (treeChanged[property]), NotificationType::dontSendNotification);
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
        String desc = KeySignature::createKeyDescription (numSharpsOrFlats, minor);
        for (int i = 0; i < keySignatureComboBox.getNumItems(); i++) {
            String val = keySignatureComboBox.getItemText (i);
            if (val == desc) {
                keySignatureComboBox.setSelectedItemIndex (i, NotificationType::dontSendNotification);
            }
        }
    }
    else if (property == transport->playHeadKeySigIsMinor.getPropertyID())
    {
        bool minor = transport->playHeadKeySigIsMinor.get();
        int numSharpsOrFlats = transport->playHeadKeySigNumSharpsOrFlats.get();
        String desc = KeySignature::createKeyDescription (numSharpsOrFlats, minor);
        for (int i = 0; i < keySignatureComboBox.getNumItems(); i++) {
            String val = keySignatureComboBox.getItemText (i);
            if (val == desc) {
                keySignatureComboBox.setSelectedItemIndex (i, NotificationType::dontSendNotification);
            }
        }
    }
    else if (property == transport->playHeadTempo.getPropertyID())
    {
        tempoSlider.setValue (transport->playHeadTempo, NotificationType::dontSendNotification);
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
        {
            transport->transportPlay (button->getToggleState());
            if (transport->recordEnabled.get())
            {
                transport->transportRecord (button->getToggleState());
            }
        }
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
        /*
        if (transport)
        {
            for (Track* track : transport->edit->tracks.objects)
            {
                track->clipList
            }
        }
         */
    }
    else if (button == &generateAccompanimentButton)
    {
        MidiFile midiFile = transport->edit->exportToMidi();
        midiFile = Interchange::callPython (midiFile);
        transport->edit->importFromMidi (midiFile, 1, 0.0);
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
    std::pair<int, bool> key = KeySignature::createRepresentationFromDescription (val);
    transport->playHeadKeySigNumSharpsOrFlats = key.first;
    transport->playHeadKeySigIsMinor = key.second;
}

void TransportView::sliderValueChanged (Slider* slider)
{
    int val = slider->getValue();
    slider->setValue (val);
    transport->playHeadTempo = val;
}
