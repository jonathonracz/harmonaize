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

struct ValueTreeIterator
{
    static ValueTreeIterator begin (const ValueTree& v) noexcept
    {
        return ValueTreeIterator (v, false);
    }

    static ValueTreeIterator end (const ValueTree& v) noexcept
    {
        return ValueTreeIterator (v, true);
    }

    typedef ValueTree value_type;
    typedef int difference_type;
    typedef ValueTree& reference;
    typedef ValueTree* pointer;
    typedef std::forward_iterator_tag iterator_category;

    ValueTreeIterator& operator++() noexcept
    {
        ++internal;
        return *this;
    }

    bool operator!= (const ValueTreeIterator& other) const noexcept
    {
        return internal != other.internal;
    }

    ValueTree operator*() const
    {
        return *internal;
    }

private:
    ValueTree::Iterator internal;

    ValueTreeIterator (const ValueTree& v, bool isEnd) noexcept
        : internal (isEnd ? v.end() : v.begin())
    {
    }
};

}
