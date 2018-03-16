/*
  ==============================================================================

    hmnz_ArrangementViewPlayHead.cpp
    Created: 16 Mar 2018 4:15:45pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_ArrangementViewPlayHead.h"
#include "hmnz_Edit.h"

ArrangementViewPlayHead::ArrangementViewPlayHead (Edit& edit)
    : ArrangementViewTimelineComponent (edit)
{
    playHeadComponent = std::unique_ptr<PlayHeadComponent> (new PlayHeadComponent);
    playHeadComponent->setBounds (0, 0, 1, getHeight());
    addAndMakeVisible (*playHeadComponent);
    setInterceptsMouseClicks (false, false);
    edit.transport.getState().addListener (this);
    edit.arrangementViewModel.getState().addListener (this);
}

ArrangementViewPlayHead::~ArrangementViewPlayHead()
{
    edit.transport.getState().removeListener (this);
    edit.arrangementViewModel.getState().removeListener (this);
}

void ArrangementViewPlayHead::resized()
{
    playHeadComponent->setBounds (playHeadComponent->getBounds().withHeight (getHeight()));
    edit.transport.getState().sendPropertyChangeMessage (edit.transport.playHeadBeat.getPropertyID());
}

void ArrangementViewPlayHead::valueTreePropertyChanged (ValueTree&, const Identifier& property)
{
    if (property == edit.transport.playHeadBeat.getPropertyID() ||
        property == edit.arrangementViewModel.timeStart.getPropertyID() ||
        property == edit.arrangementViewModel.timeEnd.getPropertyID())
    {
        playHeadComponent->setCentrePosition (getXPosForBeat (edit.transport.playHeadBeat.get()), getHeight() / 2);
    }
}
