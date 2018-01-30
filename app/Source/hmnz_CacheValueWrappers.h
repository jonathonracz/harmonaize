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
    SPSCAtomicWrapper() = default;

    template<typename OtherType>
    SPSCAtomicWrapper (const OtherType& other)
    {
        value.store (other, std::memory_order_release);
    }

    SPSCAtomicWrapper (const SPSCAtomicWrapper& other)
    {
        value.store (other.value.load (std::memory_order_acquire), std::memory_order_release);
    }

    SPSCAtomicWrapper& operator= (const SPSCAtomicWrapper& other) noexcept
    {
        HMNZ_ASSERT_IS_ON_MESSAGE_THREAD
        value.store (other.value.load (std::memory_order_acquire), std::memory_order_release);
        return *this;
    }

    bool operator== (const SPSCAtomicWrapper& other) const noexcept
    {
        bool result = value.load (std::memory_order_relaxed) == other.value.load (std::memory_order_relaxed);
        std::atomic_thread_fence (std::memory_order_acquire);
        return result;
    }

    bool operator!= (const SPSCAtomicWrapper& other) const noexcept
    {
        bool result = value.load (std::memory_order_relaxed) != other.value.load (std::memory_order_relaxed);
        std::atomic_thread_fence (std::memory_order_acquire);
        return result;
    }

    operator var() const noexcept
    {
        return value.load (std::memory_order_acquire);
    }

    operator Type() const noexcept
    {
        return value.load (std::memory_order_acquire);
    }

    std::atomic<Type> value { Type() };
};

/**
    Note that for all operations on this type which invoke a load, they must be
    followed by an atomic thread fence (acquire).
*/
template<typename Type>
struct SPSCRelaxedLoadAtomicWrapper
{
    SPSCRelaxedLoadAtomicWrapper() = default;

    template<typename OtherType>
    SPSCRelaxedLoadAtomicWrapper (const OtherType& other)
    {
        value.store (other, std::memory_order_release);
    }

    SPSCRelaxedLoadAtomicWrapper (const SPSCRelaxedLoadAtomicWrapper& other)
    {
        value.store (other.value.load (std::memory_order_acquire), std::memory_order_release);
    }

    SPSCRelaxedLoadAtomicWrapper& operator= (const SPSCRelaxedLoadAtomicWrapper& other) noexcept
    {
        HMNZ_ASSERT_IS_ON_MESSAGE_THREAD
        value.store (other.value.load (std::memory_order_acquire), std::memory_order_release);
        return *this;
    }

    bool operator== (const SPSCRelaxedLoadAtomicWrapper& other) const noexcept
    {
        bool result = value.load (std::memory_order_relaxed) == other.value.load (std::memory_order_relaxed);
        return result;
    }

    bool operator!= (const SPSCRelaxedLoadAtomicWrapper& other) const noexcept
    {
        bool result = value.load (std::memory_order_relaxed) != other.value.load (std::memory_order_relaxed);
        return result;
    }

    operator var() const noexcept
    {
        return value.load (std::memory_order_relaxed);
    }

    operator Type() const noexcept
    {
        return value.load (std::memory_order_relaxed);
    }

    std::atomic<Type> value { Type() };
};
