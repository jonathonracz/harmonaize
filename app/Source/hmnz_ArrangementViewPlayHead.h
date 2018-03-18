/*
  ==============================================================================

    hmnz_ArrangementViewPlayHead.h
    Created: 16 Mar 2018 4:15:45pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ArrangementViewTimelineComponent.h"

class ArrangementViewPlayHead   : public ArrangementViewTimelineComponent,
                                  public ValueTree::Listener
{
public:
    class PlayHeadComponent : public Component
    {
    protected:
        void paint (Graphics& g) override
        {
            g.fillAll (Colours::yellow);
        }
    };

    ArrangementViewPlayHead (Edit& edit, CachedValue<double>& timeStart, CachedValue<double>& timeEnd);
    ~ArrangementViewPlayHead();

private:
    Edit& edit;
    std::unique_ptr<PlayHeadComponent> playHeadComponent;

    void updatePlayHeadPosition();

    void resized() override;

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&) override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementViewPlayHead)
};
