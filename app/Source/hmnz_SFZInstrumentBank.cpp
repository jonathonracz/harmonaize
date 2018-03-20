/*
  ==============================================================================

    hmnz_SFZInstrumentBank.cpp
    Created: 19 Mar 2018 7:09:44pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_SFZInstrumentBank.h"
#include "hmnz_Application.h"

SFZInstrumentBank::SFZInstrumentBank()
    : Thread ("SFZ Loader Thread")
{
    startThread();
}

SFZInstrumentBank::~SFZInstrumentBank()
{
    stopThread (5000);
}

File SFZInstrumentBank::getInstrumentsDirectory()
{
    File instrumentsDirectory = File::getSpecialLocation (File::SpecialLocationType::currentApplicationFile).getChildFile("Contents/Resources/Instruments");
    jassert (instrumentsDirectory.isDirectory());
    return instrumentsDirectory;
}

SFZInstrumentBank::LoadFuture SFZInstrumentBank::loadSFZ (const File& file, std::shared_ptr<double> progress)
{
    std::promise<sfzero::Sound::Ptr> fileToLoad;
    LoadFuture ret;
    ret.future = fileToLoad.get_future();

    {
        const CriticalSection::ScopedLockType lock (sounds.getLock());
        if (sounds.contains (file))
        {
            fileToLoad.set_value (sounds[file]);
            return ret;
        }
    }

    loadQueue.enqueue ({ file, progress, std::move (fileToLoad) });
    notify();
    return ret;
}

void SFZInstrumentBank::run()
{
    LoadQueueItem queueItem;
    while (!threadShouldExit())
    {
        while (loadQueue.try_dequeue (queueItem))
        {
            {
                const CriticalSection::ScopedLockType lock (sounds.getLock());
                if (sounds.contains (queueItem.file))
                {
                    queueItem.sound.set_value (sounds[queueItem.file]);
                    break;
                }
            }

            sfzero::Sound::Ptr newSound = new sfzero::Sound (queueItem.file);
            newSound->loadRegions();
            newSound->loadSamples (&HarmonaizeApplication::getFormatManager(), queueItem.progress.get(), this);
            sounds.set (queueItem.file, newSound);
            queueItem.sound.set_value (newSound);
        }

        {
            const CriticalSection::ScopedLockType lock (sounds.getLock());
            HashMap<File, sfzero::Sound::Ptr, Utility::FileHashGenerator, CriticalSection>::Iterator it (sounds);
            Array<File> soundsToDelete;
            while (it.next())
            {
                if (it.getValue()->getReferenceCount() == 1)
                    soundsToDelete.add (it.getKey());
            }

            for (auto sound : soundsToDelete)
                sounds.remove (sound);
        }

        wait (5000);
    }
}
