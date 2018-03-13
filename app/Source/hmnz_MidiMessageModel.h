/*
  ==============================================================================

    hmnz_MidiMessageModel.h
    Created: 16 Feb 2018 2:05:30pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"
#include "hmnz_VariantConverters.h"

class MidiMessageModel  : public ValueTreeObject<IDs::MidiMessageModel>
{
public:
    MidiMessageModel (const ValueTree& v, UndoManager* um)
        : ValueTreeObject (v, um),
          bytes (getState(), IDs::MidiMessageModelProps::Bytes, getUndoManager(), MemoryBlock (MidiMessage().getRawData(), MidiMessage().getRawDataSize())),
          timestamp (getState(), IDs::MidiMessageModelProps::Timestamp, getUndoManager(), MidiMessage().getTimeStamp())
    {
    }

    MidiMessage getMessage() const
    {
        return MidiMessage (bytes.get().getData(), static_cast<int> (bytes.get().getSize()), timestamp);
    }

    void setMessage (const MidiMessage& message, double timeDelta = 0.0)
    {
        bytes = MemoryBlock (message.getRawData(), static_cast<size_t> (message.getRawDataSize()));
        timestamp = message.getTimeStamp() + timeDelta;
    }

    static ValueTree createStateForMessage (const MidiMessage& message, double timeDelta = 0.0)
    {
        ValueTree ret (identifier);
        ret.setProperty (IDs::MidiMessageModelProps::Bytes, MemoryBlock (message.getRawData(), static_cast<size_t> (message.getRawDataSize())), nullptr);
        ret.setProperty (IDs::MidiMessageModelProps::Timestamp, message.getTimeStamp() + timeDelta, nullptr);
        return ret;
    }

    CachedValue<MemoryBlock> bytes;
    CachedValue<double> timestamp;

private:
    JUCE_DECLARE_WEAK_REFERENCEABLE (MidiMessageModel)
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiMessageModel)
};
