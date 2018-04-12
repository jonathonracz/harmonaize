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
#include "hmnz_Application.h"
#include "hmnz_Genre.h"
#include "../../harmonaize/midiinterchange/hmnz_MidiInterchange.h"

TransportView::TransportView (Edit& _edit)
    : edit (_edit)
{
    goToBeginningButton.addListener (this);
    stopPlayButton.addListener (this);
    recordButton.addListener (this);
    clearButton.addListener (this);
    generateAccompanimentButton.addListener (this);
    metronomeEnabledButton.addListener (this);

    timeLabel.addListener (this);
    beatLabel.addListener (this);
    tempoSlider.addListener (this);
    timeSignatureNumerator.addListener (this);
    timeSignatureDenominator.addListener (this);
    keySignatureComboBox.addListener (this);
    genreComboBox.addListener(this);

    goToBeginningButton.setButtonText (translate ("Go to beginning"));
    stopPlayButton.setButtonText (translate ("Play"));
    recordButton.setButtonText (translate ("Record"));
    clearButton.setButtonText (translate ("Reset"));
    generateAccompanimentButton.setButtonText (translate ("Generate\nAccompaniment"));
    metronomeEnabledButton.setButtonText (translate ("Metronome"));

    recordButton.setColour (TextButton::ColourIds::buttonOnColourId, Colours::red);
    metronomeEnabledButton.setColour (TextButton::ColourIds::buttonOnColourId, Colours::green);

    timeText.setText (translate ("Seconds"), NotificationType::dontSendNotification);
    beatText.setText (translate ("Beat"), NotificationType::dontSendNotification);
    tempoText.setText (translate ("Tempo"), NotificationType::dontSendNotification);
    timeText.setEditable (false);
    beatText.setEditable (false);
    tempoText.setEditable (false);
    timeSignatureText.setText (translate ("Time"), NotificationType::dontSendNotification);
    keySignatureText.setText (translate ("Key"), NotificationType::dontSendNotification);
    genreText.setText (translate ("Genre"), NotificationType::dontSendNotification);

    tempoSlider.setColour (Slider::ColourIds::textBoxOutlineColourId, Colours::transparentWhite);
    tempoSlider.setTextBoxStyle (Slider::TextBoxBelow, 0, 80, 25);
    tempoSlider.setRange (1, 200);
    tempoSlider.setTextValueSuffix (" BPM");
    StringArray items = KeySignature::getKeyDescriptions();
    keySignatureComboBox.addItemList (items, 1);
    StringArray genres = Genre::getGenres();
    genreComboBox.addItemList (genres, 1);

    stopPlayButton.setClickingTogglesState (true);
    recordButton.setClickingTogglesState (true);
    metronomeEnabledButton.setClickingTogglesState (true);

    addAndMakeVisible (goToBeginningButton);
    addAndMakeVisible (stopPlayButton);
    addAndMakeVisible (recordButton);
    addAndMakeVisible (clearButton);
    addAndMakeVisible (generateAccompanimentButton);
    addAndMakeVisible (metronomeEnabledButton);

    addAndMakeVisible (timeLabel);
    addAndMakeVisible (beatLabel);
    addAndMakeVisible (tempoSlider);
    addAndMakeVisible (timeSignatureNumerator);
    addAndMakeVisible (timeSignatureDenominator);
    addAndMakeVisible (keySignatureComboBox);
    addAndMakeVisible(genreComboBox);

    addAndMakeVisible (timeText);
    addAndMakeVisible (beatText);
    addAndMakeVisible (tempoText);
    addAndMakeVisible (timeSignatureText);
    addAndMakeVisible (keySignatureText);
    addAndMakeVisible (genreText);

    edit.transport.getState().addListener (this);
    edit.masterTrack.getState().addListener (this);

    edit.transport.getState().sendPropertyChangeMessage (edit.transport.playState.getPropertyID());
    edit.transport.getState().sendPropertyChangeMessage (edit.transport.recordEnabled.getPropertyID());
    edit.transport.getState().sendPropertyChangeMessage (edit.transport.playHeadTime.getPropertyID());
    edit.transport.getState().sendPropertyChangeMessage (edit.transport.playHeadBeat.getPropertyID());
    edit.transport.getState().sendPropertyChangeMessage (edit.transport.playHeadTempo.getPropertyID());
    edit.transport.getState().sendPropertyChangeMessage (edit.transport.playHeadTimeSigNumerator.getPropertyID());
    edit.transport.getState().sendPropertyChangeMessage (edit.transport.playHeadTimeSigDenominator.getPropertyID());
    edit.transport.getState().sendPropertyChangeMessage (edit.transport.playHeadKeySigNumSharpsOrFlats.getPropertyID());
    edit.transport.getState().sendPropertyChangeMessage (edit.transport.playHeadKeySigIsMinor.getPropertyID());

    edit.masterTrack.getState().sendPropertyChangeMessage (edit.masterTrack.metronomeEnabled.getPropertyID());
}

TransportView::~TransportView()
{
    edit.transport.getState().removeListener (this);
    edit.masterTrack.getState().removeListener (this);
}

void TransportView::resized()
{
    FlexBox mainBox;
    mainBox.flexDirection = FlexBox::Direction::column;

    FlexBox text;
    text.flexDirection = FlexBox::Direction::row;
    timeText.setJustificationType (Justification::centred);
    beatText.setJustificationType (Justification::centred);
    tempoText.setJustificationType (Justification::centred);
    keySignatureText.setJustificationType (Justification::centred);
    timeSignatureText.setJustificationType (Justification::centred);
    genreText.setJustificationType (Justification::centred);
//    Font font = Font (20);
//    timeText.setFont (font);
//    beatText.setFont (font);
//    tempoText.setFont (font);
//    keySignatureText.setFont (font);
//    timeSignatureText.setFont (font);
    FlexItem timeT = FlexItem (timeText).withFlex (0.5f);
    FlexItem beatT = FlexItem (beatText).withFlex (0.5f);
    FlexItem tempoT = FlexItem (tempoText).withFlex (0.5f);
    FlexItem keySigT = FlexItem (keySignatureText).withFlex (0.5f);
    FlexItem timeSigT = FlexItem (timeSignatureText).withFlex (0.5f);
    FlexItem genreT = FlexItem (genreText).withFlex(0.5f);
    text.items.add (FlexItem().withFlex (1.0f));
    text.items.add (timeT);
    text.items.add (beatT);
    text.items.add (tempoT);
    text.items.add (timeSigT);
    text.items.add (keySigT);
    text.items.add (genreT);
    text.items.add (FlexItem().withFlex (1.0f));

    FlexBox labels;
    labels.flexDirection = FlexBox::Direction::row;
    timeLabel.setJustificationType (Justification::centred);
    beatLabel.setJustificationType (Justification::centred);
    timeSignatureNumerator.setJustificationType (Justification::centred);
    timeSignatureDenominator.setJustificationType (Justification::centred);
//    timeLabel.setFont (font);
//    beatLabel.setFont (font);
//    timeSignatureNumerator.setFont (font);
    timeSignatureNumerator.setEditable (true);
//    timeSignatureDenominator.setFont (font);
    timeSignatureDenominator.setEditable (true);
    FlexItem timeL = FlexItem (timeLabel).withFlex (0.5f);
    FlexItem beatL = FlexItem (beatLabel).withFlex (0.5f);
    FlexItem tempoL = FlexItem (tempoSlider).withFlex (0.5f);
    FlexItem keySigL = FlexItem (keySignatureComboBox).withFlex (0.5f);
    FlexItem genreL = FlexItem (genreComboBox).withFlex(0.5f);

    FlexBox timeSig;
    timeSig.flexDirection = FlexBox::Direction::column;
    FlexItem timeSigN = FlexItem (timeSignatureNumerator).withFlex (0.5f);
    FlexItem timeSigD = FlexItem (timeSignatureDenominator).withFlex (0.5f);
    timeSig.items.add (timeSigN);
    timeSig.items.add (timeSigD);
    FlexItem timeSigFlex = FlexItem (timeSig).withFlex (0.5f);

    labels.items.add (FlexItem().withFlex (1.0f));
    labels.items.add (timeL);
    labels.items.add (beatL);
    labels.items.add (tempoL);
    labels.items.add (timeSigFlex);
    labels.items.add (keySigL);
    labels.items.add (genreL);
    labels.items.add (FlexItem().withFlex (1.0f));

    FlexBox buttons;
    buttons.flexDirection = FlexBox::Direction::row;
    FlexItem goToBeginningB = FlexItem (goToBeginningButton).withFlex (1.0f);
    FlexItem stopPlayB = FlexItem (stopPlayButton).withFlex (1.0f);
    FlexItem recordB = FlexItem (recordButton).withFlex (1.0f);
    FlexItem clearB = FlexItem (clearButton).withFlex (1.0f);
    FlexItem generateAccompanimentB = FlexItem (generateAccompanimentButton).withFlex (1.0f);
    FlexItem metronomeEnabledB = FlexItem (metronomeEnabledButton).withFlex (1.0f);
    goToBeginningB.margin = 5;
    stopPlayB.margin = 5;
    recordB.margin = 5;
    clearB.margin = 5;
    generateAccompanimentB.margin = 5;
    metronomeEnabledB.margin = 5;
    buttons.items.add (goToBeginningB);
    buttons.items.add (stopPlayB);
    buttons.items.add (recordB);
    buttons.items.add (clearB);
    buttons.items.add (generateAccompanimentB);
    buttons.items.add (metronomeEnabledB);

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
    if (treeChanged == edit.transport.getState())
    {
        Transport& transport = edit.transport;
        if (property == transport.playState.getPropertyID())
        {
            if (int (treeChanged[property]) == Transport::State::playing)
                stopPlayButton.setToggleState (true, NotificationType::dontSendNotification);
            else if (int (treeChanged[property]) == Transport::State::stopped)
                stopPlayButton.setToggleState (false, NotificationType::dontSendNotification);
            buttonStateChanged (&stopPlayButton);
        }
        else if (property == transport.recordEnabled.getPropertyID())
        {
            recordButton.setToggleState (bool (treeChanged[property]), NotificationType::dontSendNotification);
        }
        else if (property == transport.playHeadTime.getPropertyID())
        {
            timeLabel.setText (String (static_cast<int> (transport.playHeadTime * 100.0) / 100.0), NotificationType::dontSendNotification);
        }
        else if (property == transport.playHeadBeat.getPropertyID())
        {
            beatLabel.setText (String (static_cast<int> (transport.playHeadBeat * 100.0) / 100.0), NotificationType::dontSendNotification);
        }
        else if (property == transport.playHeadKeySigNumSharpsOrFlats.getPropertyID())
        {
            bool minor = transport.playHeadKeySigIsMinor.get();
            int numSharpsOrFlats = transport.playHeadKeySigNumSharpsOrFlats.get();
            String desc = KeySignature::createKeyDescription (numSharpsOrFlats, minor);
            for (int i = 0; i < keySignatureComboBox.getNumItems(); i++)
            {
                String val = keySignatureComboBox.getItemText (i);
                if (val == desc)
                {
                    keySignatureComboBox.setSelectedItemIndex (i, NotificationType::dontSendNotification);
                }
            }
        }
        else if (property == transport.playHeadKeySigIsMinor.getPropertyID())
        {
            bool minor = transport.playHeadKeySigIsMinor.get();
            int numSharpsOrFlats = transport.playHeadKeySigNumSharpsOrFlats.get();
            String desc = KeySignature::createKeyDescription (numSharpsOrFlats, minor);
            for (int i = 0; i < keySignatureComboBox.getNumItems(); i++)
            {
                String val = keySignatureComboBox.getItemText (i);
                if (val == desc)
                {
                    keySignatureComboBox.setSelectedItemIndex (i, NotificationType::dontSendNotification);
                }
            }
        }
        else if (property == transport.playHeadTempo.getPropertyID())
        {
            tempoSlider.setValue (transport.playHeadTempo, NotificationType::dontSendNotification);
        }
        else if (property == transport.playHeadTimeSigNumerator.getPropertyID())
        {
            timeSignatureNumerator.setText (String (transport.playHeadTimeSigNumerator), NotificationType::dontSendNotification);
        }
        else if (property == transport.playHeadTimeSigDenominator.getPropertyID())
        {
            timeSignatureDenominator.setText (String (transport.playHeadTimeSigDenominator), NotificationType::dontSendNotification);
        }
    }
    else if (treeChanged == edit.masterTrack.getState())
    {
        if (property == edit.masterTrack.metronomeEnabled.getPropertyID())
        {
            metronomeEnabledButton.setToggleState (bool (treeChanged[property]), NotificationType::dontSendNotification);
        }
    }
}

void TransportView::buttonClicked (Button* button)
{
    Transport& transport = edit.transport;
    if (button == &goToBeginningButton)
    {
        transport.playHeadBeat = 0.0f;
        transport.playHeadTime = 0.0f;
//        beatLabel.setText ("0", NotificationType::dontSendNotification);
//        timeLabel.setText ("0", NotificationType::dontSendNotification);
    }
    else if (button == &stopPlayButton)
    {
        transport.playState = button->getToggleState();
        if (!button->getToggleState())
            transport.recordEnabled = button->getToggleState();
    }
    else if (button == &recordButton)
    {
        edit.getUndoManager()->beginNewTransaction ("Record");
        transport.recordEnabled = button->getToggleState();
        transport.playState = button->getToggleState();
    }
    else if (button == &clearButton)
    {
        // TODO: This belongs in some sort of arrangement controller, not transport.
        // TODO: Also, this is stupidly written and hacky
        edit.getUndoManager()->beginNewTransaction ("Clear Project");
        edit.trackList.getState().removeAllChildren (edit.getUndoManager());
        edit.trackList.tracks.addState (Track::createDefaultState());
    }
    else if (button == &generateAccompanimentButton)
    {
        edit.getUndoManager()->beginNewTransaction ("Generate Accompaniment");
        MidiFile midiFile = edit.exportToMidi();
        midiFile = Interchange::callPython (midiFile);
        edit.importFromMidi (midiFile, 1, 0.0);
    }
    else if (button == &metronomeEnabledButton)
    {
        edit.getUndoManager()->beginNewTransaction ("Toggle Metronome");
        if (edit.masterTrack.metronomeEnabled.get())
            edit.masterTrack.metronomeEnabled.setValue (false, nullptr);
        else
            edit.masterTrack.metronomeEnabled.setValue (true, nullptr);
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
    edit.getUndoManager()->beginNewTransaction ("Combo Box Change");
    String val = comboBox->getText();
    if (comboBox->getName() == "Key Signature")
    {
        std::pair<int, bool> key = KeySignature::createRepresentationFromDescription (val);
        edit.transport.playHeadKeySigNumSharpsOrFlats = key.first;
        edit.transport.playHeadKeySigIsMinor = key.second;
    }
    else
    {
        edit.masterTrack.genre.genre = Genre::getIndexFromString (val);
    }
}

void TransportView::sliderValueChanged (Slider* slider)
{
    edit.getUndoManager()->beginNewTransaction ("Tempo Change");
    int val = static_cast<int> (slider->getValue());
    slider->setValue (val);
    edit.transport.playHeadTempo = val;
}

void TransportView::labelTextChanged(Label* label)
{
    edit.getUndoManager()->beginNewTransaction ("Time Signature Change");
    if (label == &timeSignatureNumerator)
    {
        String val = label->getText();
        if (val.getIntValue() != 0)
            edit.transport.playHeadTimeSigNumerator = val.getIntValue();
        else
            label->setText (String (edit.transport.playHeadTimeSigNumerator), NotificationType::dontSendNotification);
    }
    else if (label == &timeSignatureDenominator)
    {
        String val = label->getText();
        if (val.getIntValue() != 0)
            edit.transport.playHeadTimeSigDenominator = val.getIntValue();
        else
            label->setText (String (edit.transport.playHeadTimeSigDenominator), NotificationType::dontSendNotification);
    }
}
