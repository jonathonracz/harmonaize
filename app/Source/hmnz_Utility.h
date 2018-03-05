/*
  ==============================================================================

    hmnz_Utility.h
    Created: 30 Jan 2018 12:11:42am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "hmnz_VariantConverters.h"

#define HMNZ_IS_ON_MESSAGE_THREAD \
    MessageManager::getInstance()->currentThreadHasLockedMessageManager()

#define HMNZ_ASSERT_IS_ON_MESSAGE_THREAD \
    jassert (HMNZ_IS_ON_MESSAGE_THREAD);

#define HMNZ_ASSERT_IS_NOT_ON_MESSAGE_THREAD \
    jassert (!HMNZ_IS_ON_MESSAGE_THREAD);

namespace Utility
{

template<typename Type>
void writeBackDefault (CachedValue<Type>& value) noexcept
{
    if (value.isUsingDefault())
        value = value.getDefault();
}

inline Colour randomColor() noexcept
{
    return Colour (
            static_cast<uint8> (Random::getSystemRandom().nextInt(255)),
            static_cast<uint8> (Random::getSystemRandom().nextInt(255)),
            static_cast<uint8> (Random::getSystemRandom().nextInt(255)));
}

inline void printMidiBuffer (const MidiBuffer& buffer) noexcept
{
    MidiMessage midiMessage;
    int midiMessageSamplePosition;
    MidiBuffer::Iterator midiIterator (buffer);
    if (!buffer.isEmpty())
    {
        DBG ("BEGIN MIDI BUFFER");
        while (midiIterator.getNextEvent (midiMessage, midiMessageSamplePosition))
        {
            DBG (midiMessage.getDescription());
        }
    }
}

inline int getIndexOfImmediateChildWithName (const ValueTree& parent, const Identifier& type)
{
    for (int i = 0; i < parent.getNumChildren(); ++i)
        if (parent.getChild (i).getType() == type)
            return i;

    return -1;
}

template<class Type, Type (*function)(Type)>
inline Type toNearestInterval (Type value, Type interval) noexcept
{
    return function (value / interval) * interval;
}

template<class Type>
inline Type floorToNearestInterval (Type value, Type interval) noexcept
{
    return toNearestInterval<Type, std::floor> (value, interval);
}

template<class Type>
inline Type ceilToNearestInterval (Type value, Type interval) noexcept
{
    return toNearestInterval<Type, std::ceil> (value, interval);
}

template<class Type>
inline Type roundToNearestInterval (Type value, Type interval) noexcept
{
    return toNearestInterval<Type, std::round> (value, interval);
}

}
