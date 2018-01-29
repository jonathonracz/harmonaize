/*
  ==============================================================================

    hmnz_Application.h
    Created: 28 Jan 2018 11:42:51pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_EditWindow.h"

class HarmonaizeApplication  : public JUCEApplication
{
public:
    //==============================================================================
    HarmonaizeApplication() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    //==============================================================================
    void initialise (const String& commandLine) override
    {
        editWindow = new EditWindow ();
    }

    void shutdown() override
    {
        editWindow = nullptr;
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        quit();
    }

private:
    ScopedPointer<EditWindow> editWindow;
};
