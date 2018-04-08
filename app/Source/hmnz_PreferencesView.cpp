/*
  ==============================================================================

    hmnz_PreferencesView.cpp
    Created: 18 Feb 2018 11:29:28pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_PreferencesView.h"
#include "hmnz_Application.h"

PreferencesView::PreferencesView()
    : selectorComponent (HarmonaizeApplication::getDeviceManager(), 0, 2, 0, 2, true, false, true, true)
{
    setOpaque (true);
    scaleSlider.setRange (1.0f, 2.0f, 0.1f);
    scaleSlider.setValue (1.0f);
    scaleSlider.addListener (this);
    scaleSlider.setSliderStyle (Slider::SliderStyle::IncDecButtons);
    addAndMakeVisible (selectorComponent);
    addAndMakeVisible (scaleSlider);
    addAndMakeVisible (scaleLabel);
    addKeyListener (HarmonaizeApplication::getCommandManager().getKeyMappings());
    scaleLabel.setText (translate ("UI Scale"), NotificationType::dontSendNotification);
}

void PreferencesView::resized()
{
    FlexBox mainBox;
    mainBox.flexDirection = FlexBox::Direction::column;
    FlexBox scale;
    scale.flexDirection = FlexBox::Direction::row;
    scaleLabel.setJustificationType (Justification::centred);
//    Font font = scaleLabel.getFont();
//    font.setHeight(25 * scaleSlider.getValue());
//    scaleLabel.setFont(font);
    scale.items.add (FlexItem (scaleLabel).withFlex (0.25f));
    scale.items.add (FlexItem (scaleSlider).withFlex (0.75f));

    mainBox.items.add (FlexItem (selectorComponent).withFlex (0.75f));
    mainBox.items.add (FlexItem (scale).withFlex (0.25f));

    mainBox.performLayout (getLocalBounds());
}

void PreferencesView::paint (Graphics& g)
{
    g.fillAll (Colours::grey);
}

void PreferencesView::userTriedToCloseWindow()
{
    this->setVisible (false);
}

void PreferencesView::sliderValueChanged (Slider* slider)
{
    double val = slider->getValue();
    slider->setValue (int (std::round (val * 10.0)) / 10.0);
    EditView* editView = static_cast<EditView*>(HarmonaizeApplication::getApp().editWindow.get()->getContentComponent());
    editView->customLookAndFeel->setFontScale(val);
    editView->lookAndFeelChanged();
    editView->repaint();
    repaint();
}

void PreferencesView::scaleUp()
{
    scaleSlider.setValue (scaleSlider.getValue() + 0.1f);
}

void PreferencesView::scaleDown()
{
    scaleSlider.setValue (scaleSlider.getValue() - 0.1f);
}
