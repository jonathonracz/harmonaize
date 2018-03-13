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

    Edit* getEdit() const;
    void setEdit (Edit* edit);

protected:
    virtual void editChanged (Edit* oldEdit) {}
    NormalisableRange<double> getBeatRemapper() const;
    int getXPosForBeat (double beat) const;
    double getBeatForXPos (int xPos) const;
    double getLinesPerBeatForMinimumLineSpacing (int minimumLineSpacing) const;

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override {}
    void valueTreeChildAdded (ValueTree&, ValueTree&) override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

private:
    WeakReference<Edit> edit;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementViewComponent)
};
