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
      sampleRate (getState(), IDs::TransportProps::SampleRate, nullptr, 44100.0),
      recordEnabled (getState(), IDs::TransportProps::RecordEnabled, nullptr, false),
      loopStartBeat (getState(), IDs::TransportProps::LoopStartBeat, nullptr, 0.0),
      loopEndBeat (getState(), IDs::TransportProps::LoopEndBeat, nullptr, 16.0),
      loopEnabled (getState(), IDs::TransportProps::LoopEnabled, nullptr, false)
{
    transportSource.setSource (this, 0, nullptr, sampleRate.get());
    output.setSource (&transportSource);
    HarmonaizeApplication::getDeviceManager().addAudioCallback (&output);

    keyboardState.addListener (&midiMessageCollector);
    String firstMidiInput = MidiInput::getDevices()[0];
    if (firstMidiInput.isNotEmpty())
        HarmonaizeApplication::getDeviceManager().addMidiInputCallback (firstMidiInput, &midiMessageCollector);

    getState().addListener (this);
    transportSource.addChangeListener (this);
    transportSource.setPosition (playHeadTime);
}

Transport::~Transport()
{
    HarmonaizeApplication::getDeviceManager().removeAudioCallback (&output);
    output.setSource (nullptr);
    transportSource.setSource (nullptr);
}

void Transport::setNextReadPosition (int64 newPosition)
{
    readPosition.store(newPosition, std::memory_order_release);
}

int64 Transport::getNextReadPosition() const
{
    return readPosition.load(std::memory_order_acquire);
}

int64 Transport::getTotalLength() const
{
    HMNZ_ASSERT_IS_NOT_ON_MESSAGE_THREAD
    // -1 is needed for unknown reason (probably overflow related)
    return std::numeric_limits<int64>::max() - 1;
}

double Transport::getCurrentPosition() const
{
    HMNZ_ASSERT_IS_NOT_ON_MESSAGE_THREAD
    return transportSource.getCurrentPosition();
}

double Transport::getLengthInSeconds() const
{
    HMNZ_ASSERT_IS_NOT_ON_MESSAGE_THREAD
    return transportSource.getLengthInSeconds();
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
    shouldStartPlaying ? transportSource.start() : transportSource.stop();
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

void Transport::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    std::atomic_thread_fence (std::memory_order_acquire);
    edit->prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void Transport::releaseResources()
{
    edit->releaseResources();
}

void Transport::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    double ppq = pulsesPerQuarterNote.get();
    int timeSigNumerator = edit->masterTrack->timeSignature->numerator.get();
    int timeSigDenominator = edit->masterTrack->timeSignature->denominator.get();
    double quarterNotesPerBeat = edit->masterTrack->timeSignature->quarterNotesPerBeat();
    double beat = edit->masterTrack->tempo->beat (transportSource.getCurrentPosition());

    currentPositionInfo.bpm = edit->masterTrack->tempo->tempoAtTime (transportSource.getCurrentPosition());
    currentPositionInfo.timeSigNumerator = timeSigNumerator;
    currentPositionInfo.timeSigDenominator = timeSigDenominator;
    currentPositionInfo.timeInSamples = transportSource.getNextReadPosition();
    currentPositionInfo.timeInSeconds = transportSource.getCurrentPosition();
    currentPositionInfo.editOriginTime = 0.0;
    currentPositionInfo.ppqPosition = beat * ppq * quarterNotesPerBeat;
    currentPositionInfo.ppqPositionOfLastBarStart = edit->masterTrack->timeSignature->barInTermsOfBeat (beat) * ppq * quarterNotesPerBeat;
    currentPositionInfo.frameRate = AudioPlayHead::FrameRateType::fpsUnknown;
    currentPositionInfo.isPlaying = transportSource.isPlaying();
    currentPositionInfo.isRecording = recordEnabled.get();
    currentPositionInfo.ppqLoopStart = loopStartBeat.get() * ppq * quarterNotesPerBeat;
    currentPositionInfo.ppqLoopEnd = loopEndBeat.get() * ppq * quarterNotesPerBeat;
    currentPositionInfo.isLooping = loopEnabled.get();

    edit->getNextAudioBlock (bufferToFill);

    setNextReadPosition (getNextReadPosition() + bufferToFill.numSamples);

    std::atomic_thread_fence (std::memory_order_release);
    readPositionTime.store (currentPositionInfo.timeInSeconds, std::memory_order_relaxed);
    readPositionBeat.store (beat, std::memory_order_relaxed);
    readPositionTempo.store (currentPositionInfo.bpm, std::memory_order_relaxed);
    readPositionTimeSigNumerator.store (timeSigDenominator, std::memory_order_relaxed);
    readPositionTimeSigDenominator.store (timeSigDenominator, std::memory_order_relaxed);
    readPositionKeySigNumSharpsOrFlats.store (edit->masterTrack->keySignature->numSharpsOrFlats.get(), std::memory_order_relaxed);
    readPositionKeySigIsMinor.store (edit->masterTrack->keySignature->isMinor.get(), std::memory_order_relaxed);
    triggerAsyncUpdate();
}

void Transport::changeListenerCallback (ChangeBroadcaster* source)
{
    if (transportSource.isPlaying())
        playState = State::playing;
    else
        playState = State::stopped;
}

void Transport::valueTreePropertyChanged (ValueTree& tree, const Identifier& identifier)
{
    if (tree == getState())
    {
        if (identifier == playHeadTime.getPropertyID())
        {
            double time = std::max(0.0, playHeadTime.get());
            tree.setPropertyExcludingListener (this, playHeadTime.getPropertyID(), time, nullptr);
            tree.setPropertyExcludingListener (this, playHeadBeat.getPropertyID(), edit->masterTrack->tempo->beat (time), nullptr);
            transportSource.setPosition (time);
        }
        else if (identifier == playHeadBeat.getPropertyID())
        {
            double time = std::max (0.0, edit->masterTrack->tempo->time (playHeadBeat));
            tree.setPropertyExcludingListener (this, playHeadTime.getPropertyID(), time, nullptr);
            tree.setPropertyExcludingListener (this, playHeadBeat.getPropertyID(), edit->masterTrack->tempo->beat (time), nullptr);
            transportSource.setPosition (time);
        }
        else if (identifier == playState.getPropertyID())
        {
            switch (playState)
            {
                case playing: transportPlay (true); break;
                case stopped: transportPlay (false); break;
                default: jassertfalse;
            }
        }
        else if (identifier == sampleRate.getPropertyID())
        {
            bool wasPlaying = (playState == State::playing);
            transportPlay (false);
            transportSource.setSource (this, 0, nullptr, sampleRate.get());
            std::atomic_thread_fence (std::memory_order_acquire);
            if (wasPlaying)
                transportPlay (true);
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
