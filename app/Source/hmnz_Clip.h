/*
  ==============================================================================

    hmnz_Clip.h
    Created: 22 Jan 2018 12:57:58am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"
#include "hmnz_MidiMessageModel.h"
#include "hmnz_MidiMessageSequenceModel.h"
#include "hmnz_VariantConverters.h"

class Clip  : public ValueTreeObject<IDs::Clip>
{
public:
    Clip (const ValueTree& v, UndoManager* um)
        : ValueTreeObject (v, um),
          start (getState(), IDs::ClipProps::Start, getUndoManager(), std::numeric_limits<double>::lowest()),
          length (getState(), IDs::ClipProps::Length, getUndoManager(), 0.0),
          color (getState(), IDs::ClipProps::Color, getUndoManager(), Colours::green),
          type (getState(), IDs::TrackProps::Type, getUndoManager(), IDs::ClipProps::Types::Midi),
          midiMessageSequenceModel (getState().getOrCreateChildWithName (MidiMessageSequenceModel::identifier, nullptr), getUndoManager())
    {
    }

    MidiMessageSequence getMidiMessageSequence (double timeDelta = 0.0) const noexcept
    {
        return getPartialMidiMessageSequence (std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max(), timeDelta);
    }

    MidiMessageSequence getPartialMidiMessageSequence (double rangeStart, double rangeEnd, double timeDelta = 0.0) const noexcept
    {
        return midiMessageSequenceModel.getPartialMidiMessageSequence (rangeStart, rangeEnd, timeDelta + start);
    }

    void addMidiMessageSequence (const MidiMessageSequence& sequence, double timeDelta = 0.0, bool resizeToFit = true)
    {
        if (resizeToFit)
        {
            adjustBoundsToFitMessageTimestamp (sequence.getStartTime());
            adjustBoundsToFitMessageTimestamp (sequence.getEndTime());
        }

        midiMessageSequenceModel.addMidiMessageSequence (sequence, -start + timeDelta);
    }

    void addEvent (const MidiMessage& message, double timeDelta = 0.0, bool resizeToFit = true) noexcept
    {
        if (resizeToFit)
            adjustBoundsToFitMessageTimestamp (message.getTimeStamp() + timeDelta);

        midiMessageSequenceModel.addEvent (MidiMessage (message, message.getTimeStamp() - start + timeDelta));
    }

    void setStartTimeKeepingEndTime (double newStartTime) noexcept
    {
        double startDelta = start - newStartTime;
        start = newStartTime;
        length = length + startDelta;
        midiMessageSequenceModel.addTimeToMessages (startDelta);
    }

    static ValueTree createState (double start, double length, const Colour& color, const Identifier& type)
    {
        ValueTree ret (createDefaultState());
        ret.setProperty (IDs::ClipProps::Start, start, nullptr);
        ret.setProperty (IDs::ClipProps::Length, length, nullptr);
        ret.setProperty (IDs::ClipProps::Color, VariantConverter<Colour>::toVar (color), nullptr);
        ret.setProperty (IDs::ClipProps::Type, VariantConverter<Identifier>::toVar (type), nullptr);
        return ret;
    }

    CachedValue<double> start;
    CachedValue<double> length;
    CachedValue<Colour> color;
    CachedValue<Identifier> type;

private:
    MidiMessageSequenceModel midiMessageSequenceModel;

    void adjustBoundsToFitMessageTimestamp (double timestamp) noexcept
    {
        if (timestamp < start)
            setStartTimeKeepingEndTime (timestamp);
        else if (timestamp > start + length)
            length = timestamp + length;
    }

    JUCE_DECLARE_WEAK_REFERENCEABLE (Clip)
};
