/*
  ==============================================================================

    hmnz_PlaybackEngine.cpp
    Created: 2 Mar 2018 3:22:43pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_PlaybackEngine.h"
#include "hmnz_Application.h"

PlaybackEngine::PlaybackEngine (Edit& _edit)
    : edit (_edit)
{
    HarmonaizeApplication::getDeviceManager().addChangeListener (this);
    HarmonaizeApplication::getDeviceManager().addAudioCallback (&output);

    for (int i = 1; i <= 16; ++i)
        midiStopBuffer.addEvent (MidiMessage::allSoundOff (i), 0);

    output.setSource (nullptr);
    output.setSource (this);
    edit.trackList.tracks.addListener (this);
    edit.getState().addListener (this);
    updatePositionInfo();
}

PlaybackEngine::~PlaybackEngine()
{
    edit.getState().removeListener (this);
    edit.trackList.tracks.removeListener (this);

    HarmonaizeApplication::getDeviceManager().removeAudioCallback (&output);
    output.setSource (nullptr);
}

bool PlaybackEngine::getCurrentPosition (AudioPlayHead::CurrentPositionInfo& result)
{
    result = currentPositionInfo.load (std::memory_order_acquire);
    return true;
}

void PlaybackEngine::transportPlay (bool shouldStartPlaying)
{
    HMNZ_ASSERT_IS_ON_MESSAGE_THREAD
    edit.transport.playState = (shouldStartPlaying) ? Transport::State::playing : Transport::State::stopped;
}

void PlaybackEngine::transportRecord (bool shouldStartRecording)
{
    HMNZ_ASSERT_IS_ON_MESSAGE_THREAD
    edit.transport.recordEnabled = shouldStartRecording;
}

void PlaybackEngine::transportRewind()
{
    HMNZ_ASSERT_IS_ON_MESSAGE_THREAD
}

void PlaybackEngine::setPositionSample (int64 sample)
{
    readPosition.store (sample, std::memory_order_release);
    playHeadPositionChanged.store (true, std::memory_order_release);
}

void PlaybackEngine::setPositionSecond (double second)
{
    setPositionSample (static_cast<int64> (second * activeSampleRate));
}

void PlaybackEngine::setPositionBeat (double beat)
{
    setPositionSecond (edit.masterTrack.tempo.time (beat));
}

PlaybackEngine::CurrentPositionInfo PlaybackEngine::updatePositionInfo()
{
    CurrentPositionInfo newPositionInfo;
    int64 currentReadPosition = readPosition.load (std::memory_order_acquire);
    double currentTimePosition = currentReadPosition / activeSampleRate;
    Tempo::Snapshot tempo = edit.masterTrack.tempo.snapshotAtTime (currentTimePosition);
    TimeSignature::Snapshot timeSignature = edit.masterTrack.timeSignature.getTimeSignatureAtBeat (tempo.beat);
    KeySignature::Snapshot keySignature = edit.masterTrack.keySignature.getKeySignatureAtTime (tempo.beat);

    newPositionInfo.bpm = tempo.bpm;
    newPositionInfo.timeSigNumerator = timeSignature.numerator;
    newPositionInfo.timeSigDenominator = timeSignature.denominator;
    newPositionInfo.timeInSamples = currentReadPosition;
    newPositionInfo.timeInSeconds = currentTimePosition;
    newPositionInfo.editOriginTime = 0.0;
    newPositionInfo.ppqPosition = tempo.beat * Transport::pulsesPerQuarterNote * timeSignature.quarterNotesPerBeat();
    newPositionInfo.ppqPositionOfLastBarStart = timeSignature.barInTermsOfBeat (tempo.beat);
    newPositionInfo.frameRate = AudioPlayHead::FrameRateType::fpsUnknown;
    newPositionInfo.isPlaying = static_cast<int> (edit.transport.playState.get()) == static_cast<int> (Transport::State::playing);
    newPositionInfo.isRecording = edit.transport.recordEnabled.get();
    newPositionInfo.ppqLoopStart = 0.0;
    newPositionInfo.ppqLoopEnd = std::numeric_limits<double>::max();
    newPositionInfo.isLooping = false;
    newPositionInfo.timeInBeats = tempo.beat;
    newPositionInfo.keySigNumSharpsOrFlats = keySignature.numSharpsOrFlats;
    newPositionInfo.keySigIsMinor = keySignature.isMinor;

    currentPositionInfo.store (newPositionInfo, std::memory_order_release);
    return newPositionInfo;
}

template<class Type>
void PlaybackEngine::setTransportPropertyValue (const CachedValue<Type>& property, Type value)
{
    edit.transport.getState().setPropertyExcludingListener (this, property.getPropertyID(), value, edit.transport.getUndoManager());
}

void PlaybackEngine::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    HMNZ_ASSERT_IS_ON_MESSAGE_THREAD
    activeSamplesPerBlockExpected = samplesPerBlockExpected;
    activeSampleRate = sampleRate;
    midiMessageCollector.reset (sampleRate);
    edit.prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void PlaybackEngine::releaseResources()
{
    edit.releaseResources();
}

void PlaybackEngine::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    HMNZ_ASSERT_IS_NOT_ON_MESSAGE_THREAD
    const std::lock_guard<std::mutex> lock (callbackLock);

    CurrentPositionInfo newPositionInfo = updatePositionInfo();

    MidiBuffer incomingMidi;
    midiMessageCollector.removeNextBlockOfMessages (incomingMidi, bufferToFill.numSamples);

    {
        bool trueVal = true;
        if (playHeadPositionChanged.compare_exchange_strong (trueVal, false, std::memory_order_acq_rel))
            incomingMidi.addEvents (midiStopBuffer, 0, midiStopBuffer.getNumEvents(), 0);
    }

    AudioBuffer<float> usableBufferSubset (bufferToFill.buffer->getArrayOfWritePointers(), bufferToFill.buffer->getNumChannels(), bufferToFill.startSample, bufferToFill.numSamples);
    edit.getNextAudioBlockWithInputs (usableBufferSubset, incomingMidi, *this);

    if (newPositionInfo.isPlaying)
        readPosition.fetch_add (bufferToFill.numSamples, std::memory_order_acq_rel);

    triggerAsyncUpdate();
}

void PlaybackEngine::changeListenerCallback (ChangeBroadcaster* source)
{
    if (source == &HarmonaizeApplication::getDeviceManager())
    {
        StringArray midiInputs = MidiInput::getDevices();
        for (String input : midiInputs)
        {
            if (HarmonaizeApplication::getDeviceManager().isMidiInputEnabled (input))
                HarmonaizeApplication::getDeviceManager().addMidiInputCallback (input, &midiMessageCollector);
            else
                HarmonaizeApplication::getDeviceManager().removeMidiInputCallback (input, &midiMessageCollector);
        }
    }
}

void PlaybackEngine::handleAsyncUpdate()
{
    CurrentPositionInfo currentPosition = currentPositionInfo.load (std::memory_order_acquire);
    Transport& transport = edit.transport;
    setTransportPropertyValue (transport.playHeadTime, currentPosition.timeInSeconds);
    setTransportPropertyValue (transport.playHeadBeat, currentPosition.timeInBeats);
    setTransportPropertyValue (transport.playHeadTempo, currentPosition.bpm);
    setTransportPropertyValue (transport.playHeadTimeSigNumerator, currentPosition.timeSigNumerator);
    setTransportPropertyValue (transport.playHeadTimeSigDenominator, currentPosition.timeSigDenominator);
    setTransportPropertyValue (transport.playHeadKeySigNumSharpsOrFlats, currentPosition.keySigNumSharpsOrFlats);
    setTransportPropertyValue (transport.playHeadKeySigIsMinor, currentPosition.keySigIsMinor);
}

void PlaybackEngine::valueTreePropertyChanged (ValueTree& tree, const Identifier& identifier)
{
    Transport& transport = edit.transport;
    if (tree == transport.getState())
    {
        if (identifier == transport.playHeadTime.getPropertyID())
        {
            double time = std::max (0.0, transport.playHeadTime.get());

            if (!beatSecondsRecursionGuard)
            {
                beatSecondsRecursionGuard = true;
                transport.playHeadBeat = edit.masterTrack.tempo.beat (time);
                beatSecondsRecursionGuard = false;
            }

            setPositionSecond (transport.playHeadTime);
        }
        else if (identifier == transport.playHeadBeat.getPropertyID())
        {
            double beat = std::max (0.0, transport.playHeadBeat.get());

            if (!beatSecondsRecursionGuard)
            {
                beatSecondsRecursionGuard = true;
                transport.playHeadTime = edit.masterTrack.tempo.time (beat);
                beatSecondsRecursionGuard = false;
            }

            setPositionBeat (beat);
        }
        else if (identifier == transport.recordEnabled.getPropertyID())
        {
            if (transport.recordEnabled.get())
                recordOperationID.fetch_add (1, std::memory_order_release);
        }
        else if (identifier == transport.playHeadTimeSigNumerator.getPropertyID())
        {
            edit.masterTrack.timeSignature.setNumeratorAtBeat (transport.playHeadTimeSigNumerator, transport.playHeadTime);
        }
        else if (identifier == transport.playHeadTimeSigDenominator.getPropertyID())
        {
            edit.masterTrack.timeSignature.setDenominatorAtBeat (transport.playHeadTimeSigDenominator, transport.playHeadTime);
        }
        else if (identifier == transport.playHeadTempo.getPropertyID())
        {
            edit.masterTrack.tempo.setTempoAtTime (transport.playHeadTempo, transport.playHeadTime);
        }
        else if (identifier == transport.playHeadKeySigNumSharpsOrFlats.getPropertyID())
        {
            edit.masterTrack.keySignature.setNumSharpsOrFlatsAtTime (transport.playHeadKeySigNumSharpsOrFlats, transport.playHeadTime);
        }
        else if (identifier == transport.playHeadKeySigIsMinor.getPropertyID())
        {
            edit.masterTrack.keySignature.setIsMinorAtTime (transport.playHeadKeySigIsMinor, transport.playHeadTime);
        }
    }
}

void PlaybackEngine::objectAdded (Track* track, int, HomogeneousValueTreeObjectArray<Track, CriticalSection>*)
{
    const CriticalSection::ScopedLockType lock (HarmonaizeApplication::getDeviceManager().getAudioCallbackLock());
    track->prepareToPlay (getActiveSamplesPerBlockExpected(), getActiveSampleRate());
}
