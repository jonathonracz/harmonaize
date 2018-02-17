/*
  ==============================================================================

    hmnz_MidiMessageModel.h
    Created: 16 Feb 2018 2:05:30pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"

class MidiMessageModel  : public ValueTreeObject<IDs::MidiMessageModel>
{
public:
    MidiMessageModel (const ValueTree& v, UndoManager* um)
        : ValueTreeObject (v, um),
          bytes (getState(), IDs::MidiMessageModelProps::Bytes, getUndoManager(), MemoryBlock (MidiMessage().getRawData(), MidiMessage().getRawDataSize())),
          timestamp (getState(), IDs::MidiMessageModelProps::Timestamp, getUndoManager(), MidiMessage().getTimeStamp())
    {
        getState().addListener (this);
    }

    MidiMessage getMessage() const noexcept
    {
        return MidiMessage (bytes.get().getData(), static_cast<int> (bytes.get().getSize()), timestamp);
    }

    void setMessage (const MidiMessage& message) noexcept
    {
        bytes = MemoryBlock (message.getRawData(), message.getRawDataSize());
        timestamp = message.getTimeStamp();
    }

    static ValueTree createStateForMessage (const MidiMessage& message)
    {
        ValueTree ret (identifier);
        ret.setProperty (IDs::MidiMessageModelProps::Bytes, MemoryBlock (message.getRawData(), message.getRawDataSize()), nullptr);
        ret.setProperty (IDs::MidiMessageModelProps::Timestamp, message.getTimeStamp(), nullptr);
        return ret;
    }

    CachedValue<MemoryBlock> bytes;
    CachedValue<double> timestamp;
};
