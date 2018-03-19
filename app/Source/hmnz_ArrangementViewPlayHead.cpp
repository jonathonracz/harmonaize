/*
  ==============================================================================

    hmnz_ArrangementViewPlayHead.cpp
    Created: 16 Mar 2018 4:15:45pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_ArrangementViewPlayHead.h"
#include "hmnz_Edit.h"

ArrangementViewPlayHead::ArrangementViewPlayHead (Edit& _edit, CachedValue<double>& timeStart, CachedValue<double>& timeEnd)
    : ArrangementViewTimelineComponent (timeStart, timeEnd), edit (_edit)
{
    playHeadComponent = std::unique_ptr<PlayHeadComponent> (new PlayHeadComponent);
    playHeadComponent->setBounds (0, 0, 1, getHeight());
    addAndMakeVisible (*playHeadComponent);
    setInterceptsMouseClicks (false, false);
    edit.transport.getState().addListener (this);
    timeStart.getValueTree().addListener (this);
    timeEnd.getValueTree().addListener (this);
}

ArrangementViewPlayHead::~ArrangementViewPlayHead()
{
    edit.transport.getState().removeListener (this);
    timeStart.getValueTree().removeListener (this);
    timeEnd.getValueTree().removeListener (this);
}

void ArrangementViewPlayHead::resized()
{
    playHeadComponent->setBounds (playHeadComponent->getBounds().withHeight (getHeight()));
    edit.transport.getState().sendPropertyChangeMessage (edit.transport.playHeadBeat.getPropertyID());
}

void ArrangementViewPlayHead::updatePlayHeadPosition()
{
    double currentBeat = edit.transport.playHeadBeat.get();
    playHeadComponent->setCentrePosition (getXPosForBeat (currentBeat), getHeight() / 2);
}

void ArrangementViewPlayHead::valueTreePropertyChanged (ValueTree&, const Identifier& property)
{
    if (property == edit.transport.playHeadBeat.getPropertyID() ||
        property == timeStart.getPropertyID() ||
        property == timeEnd.getPropertyID())
    {
        updatePlayHeadPosition();
    }
}
