/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "hmnz_MainComponent.h"
#include "hmnz_MidiImporter.h"

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

    activeEdit = std::unique_ptr<Edit> (new Edit (Edit::createSkeletonEdit()));
    activeEdit->setVisible (true);
    activeEdit->setSize (800, 600);
    activeEdit->setCentreRelative (0.5f, 0.5f);
    activeEdit->addToDesktop (ComponentPeer::StyleFlags::windowAppearsOnTaskbar |
                              ComponentPeer::StyleFlags::windowHasTitleBar |
                              ComponentPeer::StyleFlags::windowIsResizable |
                              ComponentPeer::StyleFlags::windowHasMinimiseButton |
                              ComponentPeer::StyleFlags::windowHasMaximiseButton |
                              ComponentPeer::StyleFlags::windowHasCloseButton |
                              ComponentPeer::StyleFlags::windowHasDropShadow);
    activeEdit->toFront (true);
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
    int amountToRemoveX = getBounds().getWidth() / 2;
    int amountToRemoveY = getBounds().getHeight() / 2;
    openEditButton.setBounds (getBounds().removeFromBottom(amountToRemoveY).removeFromLeft (amountToRemoveX));
    importMidiButton.setBounds (getBounds().removeFromBottom(amountToRemoveY).removeFromRight (amountToRemoveX));
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
