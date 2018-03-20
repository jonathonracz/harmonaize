/*
  ==============================================================================

    hmnz_SFZInstrumentBank.h
    Created: 19 Mar 2018 7:09:44pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "External/readerwriterqueue/readerwriterqueue.h"
#include <future>
#include "hmnz_Utility.h"

class SFZInstrumentBank : private Thread
{
public:
    SFZInstrumentBank();

    struct LoadFuture
    {
        std::future<sfzero::Sound::Ptr> future;
        std::shared_ptr<double> progress = std::make_shared<double> (0.0);
    };

    LoadFuture loadSFZ (const File& file);

private:
    struct LoadQueueItem
    {
        File file;
        std::shared_ptr<double> progress;
        std::promise<sfzero::Sound::Ptr> sound;
    };

    void run() override;

    HashMap<File, sfzero::Sound::Ptr, Utility::FileHashGenerator, CriticalSection> sounds;
    moodycamel::ReaderWriterQueue<LoadQueueItem> loadQueue;
};
