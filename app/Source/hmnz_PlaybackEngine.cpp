/*
  ==============================================================================

    hmnz_PlaybackEngine.cpp
    Created: 2 Mar 2018 3:22:43pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_PlaybackEngine.h"
#include "hmnz_Application.h"

PlaybackEngine::PlaybackEngine()
{
    HarmonaizeApplication::getDeviceManager().addChangeListener (this);
    HarmonaizeApplication::getDeviceManager().addAudioCallback (&output);

    for (int i = 1; i <= 16; ++i)
        midiStopBuffer.addEvent (MidiMessage::allSoundOff (i), 0);
}

PlaybackEngine::~PlaybackEngine()
{
    HarmonaizeApplication::getDeviceManager().removeAudioCallback (&output);
    output.setSource (nullptr);
}

Edit* PlaybackEngine::getEdit() const
{
    HMNZ_ASSERT_IS_ON_MESSAGE_THREAD
    return edit;
}

void PlaybackEngine::setEdit (Edit* _edit)
{
    HMNZ_ASSERT_IS_ON_MESSAGE_THREAD
    if (edit)
    {
        edit->getState().removeListener (this);
        edit->trackList.tracks.removeListener (this);
    }

    {
        const std::lock_guard<std::mutex> lock (callbackLock);
        edit = _edit;
    }

    if (edit)
    {
        output.setSource (nullptr);
        output.setSource (this);
        edit->trackList.tracks.addListener (this);
        edit->getState().addListener (this);
        updatePositionInfo();
    }
}

bool PlaybackEngine::getCurrentPosition (AudioPlayHead::CurrentPositionInfo& result)
{
    result = currentPositionInfo.load (std::memory_order_release);
    return true;
}

void PlaybackEngine::transportPlay (bool shouldStartPlaying)
{
    HMNZ_ASSERT_IS_ON_MESSAGE_THREAD
    if (edit)
        edit->transport.playState = (shouldStartPlaying) ? Transport::State::playing : Transport::State::stopped;
}

void PlaybackEngine::transportRecord (bool shouldStartRecording)
{
    HMNZ_ASSERT_IS_ON_MESSAGE_THREAD
    if (edit)
        edit->transport.recordEnabled = shouldStartRecording;
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
    setPositionSecond (edit->masterTrack.tempo.time (beat));
}

PlaybackEngine::CurrentPositionInfo PlaybackEngine::updatePositionInfo()
{
    CurrentPositionInfo newPositionInfo;
    int64 currentReadPosition = readPosition.load (std::memory_order_acquire);
    double currentTimePosition = currentReadPosition / activeSampleRate;
    Tempo::Snapshot tempo = edit->masterTrack.tempo.snapshotAtTime (currentTimePosition);
    TimeSignature::Snapshot timeSignature = edit->masterTrack.timeSignature.getTimeSignatureAtBeat (tempo.beat);
    KeySignature::Snapshot keySignature = edit->masterTrack.keySignature.getKeySignatureAtTime (tempo.beat);

    newPositionInfo.bpm = tempo.bpm;
    newPositionInfo.timeSigNumerator = timeSignature.numerator;
    newPositionInfo.timeSigDenominator = timeSignature.denominator;
    newPositionInfo.timeInSamples = currentReadPosition;
    newPositionInfo.timeInSeconds = currentTimePosition;
    newPositionInfo.editOriginTime = 0.0;
    newPositionInfo.ppqPosition = tempo.beat * Transport::pulsesPerQuarterNote * timeSignature.quarterNotesPerBeat();
    newPositionInfo.ppqPositionOfLastBarStart = timeSignature.barInTermsOfBeat (tempo.beat);
    newPositionInfo.frameRate = AudioPlayHead::FrameRateType::fpsUnknown;
    newPositionInfo.isPlaying = static_cast<int> (edit->transport.playState.get()) == static_cast<int> (Transport::State::playing);
    newPositionInfo.isRecording = edit->transport.recordEnabled.get();
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
static void setTransportPropertyValue (PlaybackEngine* engine, const CachedValue<Type>& property, Type value)
{
    jassert (engine->getEdit());
    engine->getEdit()->transport.getState().setPropertyExcludingListener (engine, property.getPropertyID(), value, engine->getEdit()->transport.getUndoManager());
}

void PlaybackEngine::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    HMNZ_ASSERT_IS_ON_MESSAGE_THREAD
    activeSamplesPerBlockExpected = samplesPerBlockExpected;
    activeSampleRate = sampleRate;
    midiMessageCollector.reset (sampleRate);

    if (edit)
        edit->prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void PlaybackEngine::releaseResources()
{
    if (edit)
        edit->releaseResources();
}

void PlaybackEngine::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    HMNZ_ASSERT_IS_NOT_ON_MESSAGE_THREAD
    const std::lock_guard<std::mutex> lock (callbackLock);

    if (!edit)
        return;

    CurrentPositionInfo newPositionInfo = updatePositionInfo();

    MidiBuffer incomingMidi;
    midiMessageCollector.removeNextBlockOfMessages (incomingMidi, bufferToFill.numSamples);

    {
        bool trueVal = true;
        if (playHeadPositionChanged.compare_exchange_strong (trueVal, false, std::memory_order_acq_rel))
            incomingMidi.addEvents (midiStopBuffer, 0, midiStopBuffer.getNumEvents(), 0);
    }

    AudioBuffer<float> usableBufferSubset (bufferToFill.buffer->getArrayOfWritePointers(), bufferToFill.buffer->getNumChannels(), bufferToFill.startSample, bufferToFill.numSamples);
    edit->getNextAudioBlockWithInputs (usableBufferSubset, incomingMidi, *this);

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
    if (!edit)
        return;

    CurrentPositionInfo currentPosition = currentPositionInfo.load (std::memory_order_acquire);
    Transport& transport = edit->transport;
    setTransportPropertyValue (this, transport.playHeadTime, currentPosition.timeInSeconds);
    setTransportPropertyValue (this, transport.playHeadBeat, currentPosition.timeInBeats);
    setTransportPropertyValue (this, transport.playHeadTempo, currentPosition.bpm);
    setTransportPropertyValue (this, transport.playHeadTimeSigNumerator, currentPosition.timeSigNumerator);
    setTransportPropertyValue (this, transport.playHeadTimeSigDenominator, currentPosition.timeSigDenominator);
    setTransportPropertyValue (this, transport.playHeadKeySigNumSharpsOrFlats, currentPosition.keySigNumSharpsOrFlats);
    setTransportPropertyValue (this, transport.playHeadKeySigIsMinor, currentPosition.keySigIsMinor);
}

void PlaybackEngine::valueTreePropertyChanged (ValueTree& tree, const Identifier& identifier)
{
    jassert (edit);
    Transport& transport = edit->transport;
    if (tree == transport.getState())
    {
        if (identifier == transport.playHeadTime.getPropertyID())
        {
            double time = std::max (0.0, transport.playHeadTime.get());
            setPositionSecond (transport.playHeadTime);
            tree.setPropertyExcludingListener (this, transport.playHeadBeat.getPropertyID(), edit->masterTrack.tempo.beat (time), nullptr);
        }
        else if (identifier == transport.playHeadBeat.getPropertyID())
        {
            double beat = std::max (0.0, transport.playHeadBeat.get());
            setPositionBeat (beat);
            tree.setPropertyExcludingListener (this, transport.playHeadTime.getPropertyID(), edit->masterTrack.tempo.time (beat), nullptr);
        }
        else if (identifier == transport.recordEnabled.getPropertyID())
        {
            if (transport.recordEnabled.get())
                recordOperationID.fetch_add (1, std::memory_order_release);
        }
        else if (identifier == transport.playHeadTimeSigNumerator.getPropertyID())
        {
            edit->masterTrack.timeSignature.setNumeratorAtBeat (transport.playHeadTimeSigNumerator, transport.playHeadTime);
        }
        else if (identifier == transport.playHeadTimeSigDenominator.getPropertyID())
        {
            edit->masterTrack.timeSignature.setDenominatorAtBeat (transport.playHeadTimeSigDenominator, transport.playHeadTime);
        }
        else if (identifier == transport.playHeadTempo.getPropertyID())
        {
            edit->masterTrack.tempo.setTempoAtTime (transport.playHeadTempo, transport.playHeadTime);
        }
        else if (identifier == transport.playHeadKeySigNumSharpsOrFlats.getPropertyID())
        {
            edit->masterTrack.keySignature.setNumSharpsOrFlatsAtTime (transport.playHeadKeySigNumSharpsOrFlats, transport.playHeadTime);
        }
        else if (identifier == transport.playHeadKeySigIsMinor.getPropertyID())
        {
            edit->masterTrack.keySignature.setIsMinorAtTime (transport.playHeadKeySigIsMinor, transport.playHeadTime);
        }
    }
}

void PlaybackEngine::objectAdded (Track* track, int, HomogeneousValueTreeObjectArray<Track, CriticalSection>*)
{
    const CriticalSection::ScopedLockType lock (HarmonaizeApplication::getDeviceManager().getAudioCallbackLock());
    track->prepareToPlay (getActiveSamplesPerBlockExpected(), getActiveSampleRate());
}