/*
  ==============================================================================

    hmnz_ArrangementViewModel.h
    Created: 4 Mar 2018 11:41:40pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"

class ArrangementViewModel  : public ValueTreeObject<IDs::ArrangementViewModel>
{
public:
    ArrangementViewModel (const ValueTree& v)
        : ValueTreeObject (v, nullptr),
          timeStart (getState(), IDs::ArrangementViewModelProps::TimeStart, getUndoManager(), 0.0),
          timeEnd (getState(), IDs::ArrangementViewModelProps::TimeEnd, getUndoManager(), 60.0)
    {
    }

    CachedValue<double> timeStart;
    CachedValue<double> timeEnd;

private:
    void valueTreePropertyChanged (ValueTree& tree, const Identifier& property) noexcept override
    {
        jassert (timeStart < timeEnd);
    }
};
