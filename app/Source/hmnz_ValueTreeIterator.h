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
    static ValueTreeForwardIterator begin (const ValueTree& v)
    {
        return ValueTreeForwardIterator (v, false);
    }

    static ValueTreeForwardIterator end (const ValueTree& v)
    {
        return ValueTreeForwardIterator (v, true);
    }

    using value_type = ValueTree;
    using difference_type = std::ptrdiff_t;
    using reference = ValueTree&;
    using pointer = ValueTree*;
    using iterator_category = std::forward_iterator_tag;

    ValueTreeForwardIterator& operator++()
    {
        ++internal;
        return *this;
    }

    bool operator!= (const ValueTreeForwardIterator& other) const
    {
        return internal != other.internal;
    }

    ValueTree operator*() const
    {
        return *internal;
    }

private:
    ValueTree::Iterator internal;

    ValueTreeForwardIterator (const ValueTree& v, bool isEnd)
        : internal (isEnd ? v.end() : v.begin())
    {
    }
};

}
