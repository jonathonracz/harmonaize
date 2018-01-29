/*
  ==============================================================================

    hmnz_Application.h
    Created: 28 Jan 2018 11:42:51pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "hmnz_EditWindow.h"

class EditWindow;

class HarmonaizeApplication  : public JUCEApplication
{
public:
    HarmonaizeApplication();
    ~HarmonaizeApplication();

    static HarmonaizeApplication& getApp();
    static AudioDeviceManager& getDeviceManager();

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    void initialise (const String& commandLine) override;
    void shutdown() override;
    void systemRequestedQuit() override;

    std::unique_ptr<EditWindow> editWindow;

private:
    AudioDeviceManager audioDeviceManager;
};
