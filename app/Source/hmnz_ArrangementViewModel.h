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

class ArrangementViewModel  : public ValueTreeObject<IDs::ArrangementViewModel>
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
    void valueTreePropertyChanged (ValueTree& tree, const Identifier& property) noexcept override
    {
        jassert (timeStart < timeEnd);
    }
};
