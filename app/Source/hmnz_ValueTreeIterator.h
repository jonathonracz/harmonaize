/*
  ==============================================================================

    hmnz_ValueTreeIterator.h
    Created: 2 Feb 2018 12:55:26am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

namespace jdr
{

struct ValueTreeForwardIterator
{
    static ValueTreeForwardIterator begin (const ValueTree& v) noexcept
    {
        return ValueTreeForwardIterator (v, false);
    }

    static ValueTreeForwardIterator end (const ValueTree& v) noexcept
    {
        return ValueTreeForwardIterator (v, true);
    }

    typedef ValueTree value_type;
    typedef int difference_type;
    typedef ValueTree& reference;
    typedef ValueTree* pointer;
    typedef std::forward_iterator_tag iterator_category;

    ValueTreeForwardIterator& operator++() noexcept
    {
        ++internal;
        return *this;
    }

    bool operator!= (const ValueTreeForwardIterator& other) const noexcept
    {
        return internal != other.internal;
    }

    ValueTree operator*() const
    {
        return *internal;
    }

private:
    ValueTree::Iterator internal;

    ValueTreeForwardIterator (const ValueTree& v, bool isEnd) noexcept
        : internal (isEnd ? v.end() : v.begin())
    {
    }
};

}