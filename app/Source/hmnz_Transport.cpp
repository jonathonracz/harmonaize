/*
  ==============================================================================

    hmnz_Transport.cpp
    Created: 28 Jan 2018 11:25:50pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_Transport.h"
#include "hmnz_Edit.h"
#include "hmnz_Application.h"

Transport::Transport (const ValueTree& v, UndoManager* um, Edit* const _edit)
    : ValueTreeObject (v, um),
      playHeadTime (getState(), IDs::TransportProps::PlayHeadTime, nullptr, 0.0),
      playHeadBeat (getState(), IDs::TransportProps::PlayHeadBeat, nullptr, 0.0),
      playHeadTempo (getState(), IDs::TransportProps::PlayHeadTempo, nullptr, 120.0),
      playHeadTimeSigNumerator (getState(), IDs::TransportProps::PlayHeadTimeSigNumerator, nullptr, 4),
      playHeadTimeSigDenominator (getState(), IDs::TransportProps::PlayHeadTimeSigDenominator, nullptr, 4),
      playHeadKeySigNumSharpsOrFlats (getState(), IDs::TransportProps::PlayHeadKeySigNumSharpsOrFlats, nullptr, 0),
      playHeadKeySigIsMinor (getState(), IDs::TransportProps::PlayHeadKeySigIsMinor, nullptr, false),
      playState (getState(), IDs::TransportProps::PlayState, nullptr, State::stopped),
      edit (_edit),
      pulsesPerQuarterNote(getState(), IDs::TransportProps::PulsesPerQuarterNote, nullptr, 960.0),
      recordEnabled (getState(), IDs::TransportProps::RecordEnabled, nullptr, false),
      loopStartBeat (getState(), IDs::TransportProps::LoopStartBeat, nullptr, 0.0),
      loopEndBeat (getState(), IDs::TransportProps::LoopEndBeat, nullptr, 16.0),
      loopEnabled (getState(), IDs::TransportProps::LoopEnabled, nullptr, false)
{
    output.setSource (this);
    HarmonaizeApplication::getDeviceManager().addChangeListener (this);
    HarmonaizeApplication::getDeviceManager().addAudioCallback (&output);

    getState().addListener (this);
}

Transport::~Transport()
{
    HarmonaizeApplication::getDeviceManager().removeAudioCallback (&output);
    output.setSource (nullptr);
}

bool Transport::getCurrentPosition (AudioPlayHead::CurrentPositionInfo& result)
{
    HMNZ_ASSERT_IS_NOT_ON_MESSAGE_THREAD
    if (!HMNZ_IS_ON_MESSAGE_THREAD)
        return false;

    result = currentPositionInfo;
    return true;
}

void Transport::transportPlay (bool shouldStartPlaying)
{
    HMNZ_ASSERT_IS_ON_MESSAGE_THREAD
    shouldStartPlaying ? playState.setValue (State::playing, nullptr) : playState.setValue (State::stopped, nullptr);
}

void Transport::transportRecord (bool shouldStartRecording)
{
    HMNZ_ASSERT_IS_ON_MESSAGE_THREAD
    recordEnabled = shouldStartRecording;
}

void Transport::transportRewind()
{
    HMNZ_ASSERT_IS_ON_MESSAGE_THREAD
}

void Transport::setPositionSample (int64 sample) noexcept
{
    readPosition.store (sample, std::memory_order_release);
}

void Transport::setPositionSecond (double second) noexcept
{
    readPosition.store (static_cast<int64> (second * getActiveSampleRate()), std::memory_order_release);
}

void Transport::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    HMNZ_ASSERT_IS_ON_MESSAGE_THREAD
    activeSamplesPerBlockExpected = samplesPerBlockExpected;
    activeSampleRate = sampleRate;
    setPositionSecond (playHeadTime);
    edit->prepareToPlay (samplesPerBlockExpected, sampleRate);
    midiMessageCollector.reset (sampleRate);
    keyboardState.reset();
}

void Transport::releaseResources()
{
    keyboardState.reset();
    edit->releaseResources();
}

void Transport::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    const std::lock_guard<std::mutex> lock (getCallbackLock());
    int64 samplePosition = readPosition.load (std::memory_order_acquire);
    double timePosition = samplePosition / getActiveSampleRate();
    double ppq = pulsesPerQuarterNote.get();
    int timeSigNumerator = edit->masterTrack.timeSignature->numerator.get();
    int timeSigDenominator = edit->masterTrack.timeSignature->denominator.get();
    double quarterNotesPerBeat = edit->masterTrack.timeSignature->quarterNotesPerBeat();
    double beat = edit->masterTrack.tempo->beat (timePosition);

    currentPositionInfo.bpm = edit->masterTrack.tempo->tempoAtTime (timePosition);
    currentPositionInfo.timeSigNumerator = timeSigNumerator;
    currentPositionInfo.timeSigDenominator = timeSigDenominator;
    currentPositionInfo.timeInSamples = samplePosition;
    currentPositionInfo.timeInSeconds = timePosition;
    currentPositionInfo.editOriginTime = 0.0;
    currentPositionInfo.ppqPosition = beat * ppq * quarterNotesPerBeat;
    currentPositionInfo.ppqPositionOfLastBarStart = edit->masterTrack.timeSignature->barInTermsOfBeat (beat) * ppq * quarterNotesPerBeat;
    currentPositionInfo.frameRate = AudioPlayHead::FrameRateType::fpsUnknown;
    currentPositionInfo.isPlaying = (static_cast<int> (playState.get()) == static_cast<int> (State::playing));
    currentPositionInfo.isRecording = recordEnabled.get();
    currentPositionInfo.ppqLoopStart = loopStartBeat.get() * ppq * quarterNotesPerBeat;
    currentPositionInfo.ppqLoopEnd = loopEndBeat.get() * ppq * quarterNotesPerBeat;
    currentPositionInfo.isLooping = loopEnabled.get();

    MidiBuffer incomingMidi;
    midiMessageCollector.removeNextBlockOfMessages (incomingMidi, bufferToFill.numSamples);
    keyboardState.processNextMidiBuffer (incomingMidi, 0, bufferToFill.numSamples, true);

    AudioBuffer<float> usableBufferSubset (bufferToFill.buffer->getArrayOfWritePointers(), bufferToFill.buffer->getNumChannels(), bufferToFill.startSample, bufferToFill.numSamples);
    edit->getNextAudioBlockWithInputs (usableBufferSubset, incomingMidi, currentPositionInfo);

    if (currentPositionInfo.isPlaying)
        readPosition.fetch_add (bufferToFill.numSamples, std::memory_order_acq_rel);

    std::atomic_thread_fence (std::memory_order_release);
    readPositionTime.store (currentPositionInfo.timeInSeconds, std::memory_order_relaxed);
    readPositionBeat.store (beat, std::memory_order_relaxed);
    readPositionTempo.store (currentPositionInfo.bpm, std::memory_order_relaxed);
    readPositionTimeSigNumerator.store (timeSigDenominator, std::memory_order_relaxed);
    readPositionTimeSigDenominator.store (timeSigDenominator, std::memory_order_relaxed);
    readPositionKeySigNumSharpsOrFlats.store (edit->masterTrack.keySignature->numSharpsOrFlats.get(), std::memory_order_relaxed);
    readPositionKeySigIsMinor.store (edit->masterTrack.keySignature->isMinor.get(), std::memory_order_relaxed);
    triggerAsyncUpdate();
}

void Transport::valueTreePropertyChanged (ValueTree& tree, const Identifier& identifier)
{
    if (tree == getState())
    {
        if (identifier == playHeadTime.getPropertyID())
        {
            double time = std::max (0.0, playHeadTime.get());
            tree.setPropertyExcludingListener (this, playHeadTime.getPropertyID(), time, nullptr);
            tree.setPropertyExcludingListener (this, playHeadBeat.getPropertyID(), edit->masterTrack.tempo->beat (time), nullptr);
            setPositionSecond (time);
        }
        else if (identifier == playHeadBeat.getPropertyID())
        {
            double time = std::max (0.0, edit->masterTrack.tempo->time (playHeadBeat));
            tree.setPropertyExcludingListener (this, playHeadTime.getPropertyID(), time, nullptr);
            tree.setPropertyExcludingListener (this, playHeadBeat.getPropertyID(), edit->masterTrack.tempo->beat (time), nullptr);
            setPositionSecond (time);
        }
        else if (identifier == playState.getPropertyID())
        {
            switch (playState.get())
            {
                case playing: transportPlay (true); break;
                case stopped: transportPlay (false); break;
                default: jassertfalse;
            }
        }
        else if (identifier == playHeadTimeSigNumerator.getPropertyID())
        {
            edit->masterTrack.timeSignature->numerator = tree[playHeadTimeSigNumerator.getPropertyID()];
        }
        else if (identifier == playHeadTimeSigDenominator.getPropertyID())
        {
            edit->masterTrack.timeSignature->denominator = tree[playHeadTimeSigDenominator.getPropertyID()];
        }
        else if (identifier == playHeadTempo.getPropertyID())
        {
            edit->masterTrack.tempo->automation.markers.objects[0]->value = tree[playHeadTempo.getPropertyID()];
        }
        else if (identifier == playHeadKeySigNumSharpsOrFlats.getPropertyID())
        {
            edit->masterTrack.keySignature->numSharpsOrFlats = tree[playHeadKeySigNumSharpsOrFlats.getPropertyID()];
        }
        else if (identifier == playHeadKeySigIsMinor.getPropertyID())
        {
            edit->masterTrack.keySignature->isMinor = tree[playHeadKeySigIsMinor.getPropertyID()];
        }
    }
}

void Transport::handleAsyncUpdate()
{
    getState().setPropertyExcludingListener (this, playHeadTime.getPropertyID(), readPositionTime.load (std::memory_order_relaxed), nullptr);
    getState().setPropertyExcludingListener (this, playHeadBeat.getPropertyID(), readPositionBeat.load (std::memory_order_relaxed), nullptr);
    getState().setPropertyExcludingListener (this, playHeadTempo.getPropertyID(), readPositionTempo.load (std::memory_order_relaxed), nullptr);
    getState().setPropertyExcludingListener (this, playHeadTimeSigNumerator.getPropertyID(), readPositionTimeSigNumerator.load (std::memory_order_relaxed), nullptr);
    getState().setPropertyExcludingListener (this, playHeadTimeSigDenominator.getPropertyID(), readPositionTimeSigDenominator.load (std::memory_order_relaxed), nullptr);
    getState().setPropertyExcludingListener (this, playHeadKeySigNumSharpsOrFlats.getPropertyID(), readPositionKeySigNumSharpsOrFlats.load (std::memory_order_relaxed), nullptr);
    getState().setPropertyExcludingListener (this, playHeadKeySigIsMinor.getPropertyID(), readPositionKeySigIsMinor.load (std::memory_order_relaxed), nullptr);
    std::atomic_thread_fence (std::memory_order_acquire);
}

void Transport::changeListenerCallback (ChangeBroadcaster* source)
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
