/*
  ==============================================================================

    hmnz_ArrayIterator.h
    Created: 17 Feb 2018 4:12:22pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

namespace jdr
{

template<class Type>
struct ArrayForwardIteratorConst
{
    static ArrayForwardIteratorConst begin (const Array<Type>& a) noexcept
    {
        return ArrayForwardIteratorConst (a, false);
    }

    static ArrayForwardIteratorConst end (const Array<Type>& a) noexcept
    {
        return ArrayForwardIteratorConst (a, true);
    }

    using value_type = Array<Type>;
    using difference_type = std::ptrdiff_t;
    using reference = Array<Type>&;
    using pointer = Array<Type>*;
    using iterator_category = std::forward_iterator_tag;

    ArrayForwardIteratorConst& operator++() noexcept
    {
        ++internal;
        return *this;
    }

    bool operator== (const ArrayForwardIteratorConst& other) const noexcept
    {
        return internal == other.internal;
    }

    bool operator!= (const ArrayForwardIteratorConst& other) const noexcept
    {
        return internal != other.internal;
    }

    Type operator*() const
    {
        return *internal;
    }

    Type operator->() const
    {
        return *internal;
    }

private:
    Type* internal;

    ArrayForwardIteratorConst (const Array<Type>& a, bool isEnd) noexcept
        : internal (isEnd ? a.end() : a.begin())
    {
    }
};

template<class Type>
struct OwnedArrayForwardIteratorConst
{
    static OwnedArrayForwardIteratorConst begin (const OwnedArray<Type>& a) noexcept
    {
        return OwnedArrayForwardIteratorConst (a, false);
    }

    static OwnedArrayForwardIteratorConst end (const OwnedArray<Type>& a) noexcept
    {
        return OwnedArrayForwardIteratorConst (a, true);
    }

    using value_type = OwnedArray<Type>;
    using difference_type = std::ptrdiff_t;
    using reference = OwnedArray<Type>&;
    using pointer = OwnedArray<Type>*;
    using iterator_category = std::forward_iterator_tag;

    OwnedArrayForwardIteratorConst& operator++() noexcept
    {
        ++internal;
        return *this;
    }

    bool operator== (const OwnedArrayForwardIteratorConst& other) const noexcept
    {
        return internal == other.internal;
    }

    bool operator!= (const OwnedArrayForwardIteratorConst& other) const noexcept
    {
        return internal != other.internal;
    }

    Type* operator*() const
    {
        return *internal;
    }

    Type* operator->() const
    {
        return *internal;
    }

private:
    Type** internal;

    OwnedArrayForwardIteratorConst (const OwnedArray<Type>& a, bool isEnd) noexcept
        : internal (isEnd ? a.end() : a.begin())
    {
    }
};

}
