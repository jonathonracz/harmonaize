/*
  ==============================================================================

    hmnz_Clip.cpp
    Created: 11 Mar 2018 12:25:53pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_Clip.h"
#include "hmnz_Track.h"

Clip::Clip (const ValueTree& v, UndoManager* um, Track& _track)
    : ValueTreeObject (v, um),
      start (getState(), IDs::ClipProps::Start, getUndoManager(), std::numeric_limits<double>::lowest()),
      length (getState(), IDs::ClipProps::Length, getUndoManager(), 0.0),
      color (getState(), IDs::ClipProps::Color, getUndoManager(), _track.color),
      type (getState(), IDs::TrackProps::Type, getUndoManager(), defaultClipTypeForTrackType (_track.type)),
      midiMessageSequenceModel (getState().getOrCreateChildWithName (MidiMessageSequenceModel::identifier, nullptr), getUndoManager()),
      track (_track)
{
}

MidiMessageSequence Clip::getMidiMessageSequence (double timeDelta) const
{
    return getPartialMidiMessageSequence (std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max(), timeDelta);
}

MidiMessageSequence Clip::getPartialMidiMessageSequence (double rangeStart, double rangeEnd, double timeDelta) const
{
    return midiMessageSequenceModel.getPartialMidiMessageSequence (rangeStart, rangeEnd, timeDelta + start);
}

void Clip::setMidiMessageSequence (const MidiMessageSequence& sequence, double timeDelta, bool resizeToFit)
{
    if (resizeToFit)
    {
        adjustBoundsToFitMessageTimestamp (sequence.getStartTime());
        adjustBoundsToFitMessageTimestamp (sequence.getEndTime());
    }

    midiMessageSequenceModel.setMidiMessageSequence (sequence, -start + timeDelta);
}

void Clip::addMidiMessageSequence (const MidiMessageSequence& sequence, double timeDelta, bool resizeToFit)
{
    if (resizeToFit)
    {
        adjustBoundsToFitMessageTimestamp (sequence.getStartTime());
        adjustBoundsToFitMessageTimestamp (sequence.getEndTime());
    }

    midiMessageSequenceModel.addMidiMessageSequence (sequence, -start + timeDelta);
}

void Clip::addEvent (const MidiMessage& message, double timeDelta, bool resizeToFit)
{
    if (resizeToFit)
        adjustBoundsToFitMessageTimestamp (message.getTimeStamp() + timeDelta);

    midiMessageSequenceModel.addEvent (MidiMessage (message, message.getTimeStamp() - start + timeDelta));
}

void Clip::setStartTimeKeepingEndTime (double newStartTime)
{
    double startDelta = start - newStartTime;
    start = newStartTime;
    length = length.get() + startDelta;
    midiMessageSequenceModel.addTimeToMessages (startDelta);
}

ValueTree Clip::createState (double start, double length, const Identifier& type, const Colour& color)
{
    ValueTree ret (createDefaultState());
    ret.setProperty (IDs::ClipProps::Start, start, nullptr);
    ret.setProperty (IDs::ClipProps::Length, length, nullptr);
    ret.setProperty (IDs::ClipProps::Color, VariantConverter<Colour>::toVar (color), nullptr);
    ret.setProperty (IDs::ClipProps::Type, VariantConverter<Identifier>::toVar (type), nullptr);
    return ret;
}

Identifier Clip::defaultClipTypeForTrackType (const Identifier& trackType)
{
    if (trackType == IDs::TrackProps::Types::Midi)
        return IDs::ClipProps::Types::Midi;

    return Identifier();
}

ValueTree Clip::createState (double start, double length, const MidiMessageSequence& sequence, const Colour& color)
{
    ValueTree ret = createState (start, length, IDs::ClipProps::Types::Midi, color);
    MidiMessageSequenceModel midiSeq (ret.getOrCreateChildWithName (MidiMessageSequenceModel::identifier, nullptr), nullptr);
    midiSeq.addMidiMessageSequence (sequence);

    return ret;
}

void Clip::adjustBoundsToFitMessageTimestamp (double timestamp)
{
    if (timestamp < start)
        setStartTimeKeepingEndTime (timestamp);
    else if (timestamp > start + length)
        length = timestamp + length;
}
