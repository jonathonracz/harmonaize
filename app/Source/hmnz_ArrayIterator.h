/*
  ==============================================================================

    hmnz_ArrayIterator.h
    Created: 17 Feb 2018 4:12:22pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

namespace jdr
{

template<class Type>
struct ArrayForwardIterator
{
    static ArrayForwardIterator begin (const Array<Type>& a) noexcept
    {
        return ArrayForwardIterator (a, false);
    }

    static ArrayForwardIterator end (const Array<Type>& a) noexcept
    {
        return ArrayForwardIterator (a, true);
    }

    using value_type = Array<Type>;
    using difference_type = std::ptrdiff_t;
    using reference = Array<Type>&;
    using pointer = Array<Type>*;
    using iterator_category = std::forward_iterator_tag;

    ArrayForwardIterator& operator++() noexcept
    {
        ++internal;
        return *this;
    }

    bool operator== (const ArrayForwardIterator& other) const noexcept
    {
        return internal == other.internal;
    }

    bool operator!= (const ArrayForwardIterator& other) const noexcept
    {
        return internal != other.internal;
    }

    Type operator*() const
    {
        return *internal;
    }

private:
    Type* internal;

    ArrayForwardIterator (const Array<Type>& a, bool isEnd) noexcept
        : internal (isEnd ? a.end() : a.begin())
    {
    }
};

}
