/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "hmnz_MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent()
{
    setSize (600, 400);
    openEditButton.addListener (this);
    importMidiButton.addListener (this);

    openEditButton.setButtonText ("Open Edit");
    importMidiButton.setButtonText ("Import MIDI");

    addAndMakeVisible (openEditButton);
    addAndMakeVisible (importMidiButton);
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
}

void MainContentComponent::resized()
{
    openEditButton.setBounds (getBounds().removeFromLeft (getBounds().getWidth() / 2));
    importMidiButton.setBounds (getBounds().removeFromRight (getBounds().getWidth() / 2));
}

void MainContentComponent::buttonClicked (Button* button)
{
    if (button == &openEditButton)
    {
        FileChooser editFileChooser ("Open Edit", File(), "*.xml");
        if (editFileChooser.browseForFileToOpen())
        {
            DBG (editFileChooser.getResult().getFileName() << " picked");
        }
    }
    else if (button == &importMidiButton)
    {
        FileChooser importMidiChooser ("Import MIDI", File(), "*.midi;*.mid");
        if (importMidiChooser.browseForFileToOpen())
        {
            DBG (importMidiChooser.getResult().getFileName() << " picked");
        }
    }
}
