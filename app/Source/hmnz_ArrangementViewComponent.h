/*
  ==============================================================================

    hmnz_ArrangementViewComponent.h
    Created: 7 Mar 2018 1:17:01pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"

class Edit;

class ArrangementViewComponent  : public Component,
                                  public ValueTree::Listener
{
public:
    ArrangementViewComponent() = default;

    Edit* getEdit() const noexcept;
    void setEdit (Edit* edit) noexcept;

protected:
    virtual void editChanged (Edit* oldEdit) noexcept {}
    NormalisableRange<double> getBeatRemapper() const noexcept;
    int getXPosForBeat (double beat) const noexcept;
    double getBeatForXPos (int xPos) const noexcept;
    double getLinesPerBeatForMinimumLineSpacing (int minimumLineSpacing) const noexcept;

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override {}
    void valueTreeChildAdded (ValueTree&, ValueTree&) override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

private:
    WeakReference<Edit> edit;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementViewComponent)
};
