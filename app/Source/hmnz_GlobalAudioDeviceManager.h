/*
  ==============================================================================

    hmnz_GlobalAudioDeviceManager.h
    Created: 26 Jan 2018 2:15:44pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

class GlobalAudioDeviceManager
{
public:
    GlobalAudioDeviceManager()
    {
        deviceManager.initialiseWithDefaultDevices (0, 0);
    }

    const AudioDeviceManager* getDeviceManager()
    {
        return &deviceManager;
    }

    juce_DeclareSingleton (GlobalAudioDeviceManager, false)

private:
    AudioDeviceManager deviceManager;
};

juce_ImplementSingleton (GlobalAudioDeviceManager)
