/*
  ==============================================================================

    hmnz_Application.cpp
    Created: 28 Jan 2018 11:42:51pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_Application.h"

HarmonaizeApplication& HarmonaizeApplication::getApp()
{
    HarmonaizeApplication* const app = static_cast<HarmonaizeApplication*> (HarmonaizeApplication::getInstance());
    return *app;
}

AudioDeviceManager& HarmonaizeApplication::getDeviceManager()
{
    return HarmonaizeApplication::getApp().audioDeviceManager;
}

void HarmonaizeApplication::initialise (const String& commandLine)
{
    editWindow = std::unique_ptr<EditWindow> (new EditWindow());
}

void HarmonaizeApplication::shutdown()
{
    editWindow = nullptr;
}

void HarmonaizeApplication::systemRequestedQuit()
{
    quit();
}
