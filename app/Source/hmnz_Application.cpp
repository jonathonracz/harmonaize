/*
  ==============================================================================

    hmnz_Application.cpp
    Created: 28 Jan 2018 11:42:51pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_Application.h"

HarmonaizeApplication::HarmonaizeApplication()
{
    unitTestRunner.runAllTests();

    // TODO: Hardcoded yuckiness here
    audioDeviceManager.initialiseWithDefaultDevices (0, 2);
    audioFormatManager.registerBasicFormats();
}

HarmonaizeApplication::~HarmonaizeApplication()
{
}

HarmonaizeApplication& HarmonaizeApplication::getApp()
{
    HarmonaizeApplication* const app = static_cast<HarmonaizeApplication*> (JUCEApplication::getInstance());
    return *app;
}

AudioDeviceManager& HarmonaizeApplication::getDeviceManager()
{
    return HarmonaizeApplication::getApp().audioDeviceManager;
}

AudioFormatManager& HarmonaizeApplication::getFormatManager()
{
    return HarmonaizeApplication::getApp().audioFormatManager;
}

ApplicationCommandManager& HarmonaizeApplication::getCommandManager()
{
    return getApp().commandManager;
}

File HarmonaizeApplication::getInstrumentsDirectory()
{
    File instrumentsDirectory = File::getSpecialLocation (File::SpecialLocationType::currentApplicationFile).getChildFile("../../../../../Source/Instruments");
    jassert (instrumentsDirectory.isDirectory());
    return instrumentsDirectory;
}

SFZInstrumentBank& HarmonaizeApplication::getInstrumentBank()
{
    return getApp().instrumentBank;
}

void HarmonaizeApplication::initialise (const String& commandLine)
{
    editWindow = std::unique_ptr<EditWindow> (new EditWindow());
    preferencesView = std::unique_ptr<PreferencesView> (new PreferencesView());
    preferencesView->centreWithSize (400, 350);
    preferencesView->setName("Preferences");
    preferencesView->addToDesktop (
        ComponentPeer::StyleFlags::windowAppearsOnTaskbar |
        ComponentPeer::StyleFlags::windowHasTitleBar |
        ComponentPeer::StyleFlags::windowHasMinimiseButton |
        ComponentPeer::StyleFlags::windowHasCloseButton |
        ComponentPeer::StyleFlags::windowHasDropShadow);
    commandManager.registerAllCommandsForTarget (this);
    commandManager.registerAllCommandsForTarget (editWindow.get());
    editWindow->menuItemsChanged();
}

void HarmonaizeApplication::shutdown()
{
    editWindow.reset();
    preferencesView.reset();
    audioDeviceManager.closeAudioDevice();
}

void HarmonaizeApplication::systemRequestedQuit()
{
    quit();
}
