/*
  ==============================================================================

    hmnz_ArrangementViewModel.h
    Created: 4 Mar 2018 11:41:40pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"
#include "hmnz_CacheValueWrappers.h"

class ArrangementViewModel  : public ValueTreeObject<IDs::ArrangementViewModel>,
                              public ValueTree::Listener
{
public:
    ArrangementViewModel (const ValueTree& v)
        : ValueTreeObject (v, nullptr),
          timeStart (getState(), IDs::ArrangementViewModelProps::TimeStart, nullptr, 0.0),
          timeEnd (getState(), IDs::ArrangementViewModelProps::TimeEnd, nullptr, 60.0),
          headerWidth (getState(), IDs::ArrangementViewModelProps::HeaderWidth, nullptr, 64),
          scrollPosition (getState(), IDs::ArrangementViewModelProps::ScrollPosition, nullptr, 0.0)
    {
    }

    CachedValue<double> timeStart;
    CachedValue<double> timeEnd;
    CachedValue<MinMaxConstrainerWrapper<int, 64, 128>> headerWidth;
    CachedValue<MinMaxConstrainerWrapper<int, 0, std::numeric_limits<int>::max()>> scrollPosition;

private:
    void valueTreePropertyChanged (ValueTree& tree, const Identifier& property) override
    {
        jassert (timeStart < timeEnd);
    }

    void valueTreeChildAdded (ValueTree&, ValueTree&) override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementViewModel)
};
