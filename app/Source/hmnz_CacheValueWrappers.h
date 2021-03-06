/*
  ==============================================================================

    hmnz_CacheValueWrappers.h
    Created: 29 Jan 2018 11:55:20pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "hmnz_Utility.h"

/**
    Based on the AtomicWrapper class from David Rowland's ADC 2017 talk.
*/
template<typename Type>
struct SPSCAtomicWrapper
{
    template<typename OtherType>
    SPSCAtomicWrapper (const OtherType& other)
    {
        value.store (other, std::memory_order_release);
    }

    SPSCAtomicWrapper (const SPSCAtomicWrapper& other)
    {
        value.store (other.value.load (std::memory_order_acquire), std::memory_order_release);
    }

    SPSCAtomicWrapper& operator= (const SPSCAtomicWrapper& other)
    {
        HMNZ_ASSERT_IS_ON_MESSAGE_THREAD
        value.store (other.value.load (std::memory_order_acquire), std::memory_order_release);
        return *this;
    }

    bool operator== (const SPSCAtomicWrapper& other) const
    {
        bool result = value.load (std::memory_order_relaxed) == other.value.load (std::memory_order_relaxed);
        std::atomic_thread_fence (std::memory_order_acquire);
        return result;
    }

    bool operator!= (const SPSCAtomicWrapper& other) const
    {
        bool result = value.load (std::memory_order_relaxed) != other.value.load (std::memory_order_relaxed);
        std::atomic_thread_fence (std::memory_order_acquire);
        return result;
    }

    operator var() const
    {
        return value.load (std::memory_order_acquire);
    }

    operator Type() const
    {
        return value.load (std::memory_order_acquire);
    }

    Type getRelaxed() const
    {
        return value.load (std::memory_order_relaxed);
    }

    void setRelaxed (Type newValue)
    {
        value.store (newValue, std::memory_order_relaxed);
    }

    std::atomic<Type> value { Type() };
};

/**
    Doesn't actually constrain.
*/
template<typename Type>
struct DefaultConstrainer
{
    static Type constrain (const Type& value)
    {
        return value;
    }
};

template<typename Type, Type min, Type max>
struct MinMaxConstrainer
{
    static Type constrain (const Type& value)
    {
        return std::min (std::max (min, value), max);
    }
};

template<typename Type, class Constrainer = DefaultConstrainer<Type>>
struct ConstrainerWrapper
{
    template<typename OtherType>
    ConstrainerWrapper (const OtherType& other)
    {
        value = Constrainer::constrain (other);
    }

    ConstrainerWrapper& operator= (const ConstrainerWrapper& other)
    {
        value = Constrainer::constrain (other.value);
        return *this;
    }

    bool operator== (const ConstrainerWrapper& other) const
    {
        return value == other.value;
    }

    bool operator!= (const ConstrainerWrapper& other) const
    {
        return value != other.value;
    }

    operator var() const
    {
        return Constrainer::constrain (value);
    }

    operator Type() const
    {
        return Constrainer::constrain (value);
    }

    Type value = Type();
};

template<typename Type, Type min, Type max>
using MinMaxConstrainerWrapper = ConstrainerWrapper<Type, MinMaxConstrainer<Type, min, max>>;
