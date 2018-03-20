/*
  ==============================================================================

    hmnz_Track.cpp
    Created: 28 Jan 2018 12:32:09pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_Track.h"
#include "hmnz_Application.h"

Track::Track (const ValueTree& v, UndoManager* um, Edit& _edit)
    : ValueTreeObject (v, um),
      name (getState(), IDs::TrackProps::Name, getUndoManager(), "New Track"),
      color (getState(), IDs::TrackProps::Color, getUndoManager(), Utility::randomColor()),
      type (getState(), IDs::TrackProps::Type, getUndoManager(), IDs::TrackProps::Types::Midi),
      height (getState(), IDs::TrackProps::Height, nullptr, 48),
      recordArmed (getState(), IDs::TrackProps::RecordArmed, nullptr, false),
      edit (_edit),
      clipList (getState().getOrCreateChildWithName (IDs::ClipList, nullptr), getUndoManager(), *this)
{
    // May not entirely prevent allocation in the audio callback, but it will
    // at least provide a good starting point. 2048 is what's used internally
    // by JUCE.
    midiBuffer.ensureSize (2048);

    getState().addListener (this);

    for (int i = 0; i < 16; ++i)
        synthesizer.addVoice (new sfzero::Voice);

    File instrumentsDirectory = SFZInstrumentBank::getInstrumentsDirectory();
    instrumentsDirectory = instrumentsDirectory.getChildFile ("./VSCO-2-CE/UprightPiano.sfz");
    loadSFZInstrument (instrumentsDirectory);
}

void Track::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    flushMidiWriteBackQueue();
    midiWriteBackQueue = moodycamel::ReaderWriterQueue<RecordedMidiMessage> (static_cast<size_t> (sampleRate)); // Buffer 1 second of messages
    audioBuffer.setSize (2, samplesPerBlockExpected); // TODO: Hardcoded number of channels.
    synthesizer.setCurrentPlaybackSampleRate (sampleRate);
    isPrepared.store (true, std::memory_order_release);
}

void Track::releaseResources()
{
}

void Track::getNextAudioBlockWithInputs (AudioBuffer<float>& audioBuffer,
                                  MidiBuffer& incomingMidiBuffer,
                                  PlaybackEngine& playbackSource)
{
    if (!isPrepared.load (std::memory_order_acquire))
        return;

    midiBuffer = incomingMidiBuffer;

    AudioPlayHead::CurrentPositionInfo currentPosition;
    bool result = playbackSource.getCurrentPosition (currentPosition);
    jassert (result);

    if (currentPosition.isPlaying)
    {
        auto beatForSample = [&](int64 sample) -> double {
            return edit.masterTrack.tempo.beat (sample / playbackSource.getActiveSampleRate());
        };

        auto sampleForBeat = [&](double beat) -> int64 {
            return static_cast<int64> (edit.masterTrack.tempo.time (beat) * playbackSource.getActiveSampleRate());
        };

        AudioPlayHead::CurrentPositionInfo positionInfo;
        bool result = playbackSource.getCurrentPosition (positionInfo);
        jassert (result);

        // Pull MIDI data for this frame from the active sequence.
        double startBeat = beatForSample (positionInfo.timeInSamples);
        double endBeat = beatForSample (positionInfo.timeInSamples + audioBuffer.getNumSamples());

        int startBeatSequenceIndex = midiReadCache.getNextIndexAtTime (startBeat);
        int endBeatSequenceIndex = midiReadCache.getNextIndexAtTime (endBeat);
        for (int i = startBeatSequenceIndex; i < endBeatSequenceIndex; ++i)
        {
            MidiMessage messageToPlay = midiReadCache.getEventPointer (i)->message;
            midiBuffer.addEvent (messageToPlay, static_cast<int> (sampleForBeat (messageToPlay.getTimeStamp())));
        }

        // Write back any incoming MIDI if we're recording.
        if (positionInfo.isRecording )//&& recordArmed.get())
        {
            RecordedMidiMessage newRecordedMessage;
            newRecordedMessage.recordSessionID = playbackSource.getRecordOperationID();

            MidiBuffer::Iterator midiBufferIt (incomingMidiBuffer);
            MidiMessage incomingMessage;
            int incomingMessagePosition;
            while (midiBufferIt.getNextEvent (incomingMessage, incomingMessagePosition))
            {
                newRecordedMessage.message = MidiMessage (incomingMessage, beatForSample (incomingMessagePosition + positionInfo.timeInSamples));
                midiWriteBackQueue.enqueue (newRecordedMessage);
            }

            triggerAsyncUpdate(); // Trigger writeback on the message thread.
        }
    }

    synthesizer.renderNextBlock (audioBuffer, midiBuffer, 0, audioBuffer.getNumSamples());
}

MidiMessageSequence Track::getMidiMessageSequence() const
{
    MidiMessageSequence ret;
    for (Clip* clip : clipList.clips)
    {
        if (clip->type == IDs::ClipProps::Types::Midi && clip->midiMessageSequenceModel.midiMessages.size() > 0)
        {
            ret.addSequence (clip->getMidiMessageSequence(), 0.0);
        }
    }

    return ret;
}

void Track::addMidiMessageSequenceAsClip (double start, double length, const MidiMessageSequence& sequence)
{
    clipList.clips.insertStateAtObjectIndex (Clip::createState (start, length, sequence, color), -1);
}

void Track::loadSFZInstrument (const File& sfzInstrument)
{
    synthesizer.clearSounds();
    loadFuture = HarmonaizeApplication::getInstrumentBank().loadSFZ (sfzInstrument, loadProgress);
    startTimer (1000);
}

void Track::updateMidiReadCache()
{
    HMNZ_ASSERT_IS_ON_MESSAGE_THREAD
    MidiMessageSequence newReadCache = getMidiMessageSequence();

    const CriticalSection::ScopedLockType lock (HarmonaizeApplication::getDeviceManager().getAudioCallbackLock());
    midiReadCache.swapWith (newReadCache);
}

void Track::flushMidiWriteBackQueue()
{
    HMNZ_ASSERT_IS_ON_MESSAGE_THREAD
    for (size_t i = 0; i < midiWriteBackQueue.size_approx(); ++i)
    {
        RecordedMidiMessage pulledMessage;
        if (!midiWriteBackQueue.try_dequeue (pulledMessage))
            break;

        if (!currentRecordClip || pulledMessage.recordSessionID != currentRecordSessionID)
        {
            jassert (pulledMessage.recordSessionID > currentRecordSessionID);
            currentRecordSessionID = pulledMessage.recordSessionID;
            currentRecordClip = clipList.clipAtTime (pulledMessage.message.getTimeStamp());

            if (!currentRecordClip)
            {
                ValueTree newClipState = Clip::createState (pulledMessage.message.getTimeStamp(), 1.0, IDs::ClipProps::Types::Midi, color);
                clipList.getState().addChild (newClipState, -1, getUndoManager());
                currentRecordClip = clipList.clips.objectWithState (newClipState);
            }
        }

        jassert (currentRecordClip);
        currentRecordClip->addEvent (pulledMessage.message);
    }
}

void Track::handleAsyncUpdate()
{
    flushMidiWriteBackQueue();
}

void Track::valueTreePropertyChanged (ValueTree& tree, const Identifier& property)
{
    // TODO: This can be specialized a lot more to filter when we do this expensive operation.
    updateMidiReadCache();
    if (tree.isAChildOf (clipList.getState()))
    {
        // TODO: if statements are missing cases
    }
}

void Track::valueTreeChildAdded (ValueTree& parent, ValueTree& childAdded)
{
    updateMidiReadCache();
    if (parent == clipList.getState())
    {
        // TODO: if statements are missing cases
    }
}

void Track::valueTreeChildRemoved (ValueTree& parent, ValueTree& removedChild, int)
{
    updateMidiReadCache();
    if (parent == clipList.getState())
    {
        // TODO: if statements are missing cases
    }
}

void Track::timerCallback()
{
    if (loadFuture.future.wait_for (std::chrono::seconds (0)) == std::future_status::ready)
    {
        synthesizer.addSound (loadFuture.future.get());
        stopTimer();
    }
}
