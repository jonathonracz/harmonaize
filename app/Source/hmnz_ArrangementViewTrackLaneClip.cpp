/*
  ==============================================================================

    hmnz_ArrangementViewTrackLaneClip.cpp
    Created: 11 Mar 2018 12:03:40pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_ArrangementViewTrackLaneClip.h"
#include "hmnz_Clip.h"

ArrangementViewTrackLaneClip::ArrangementViewTrackLaneClip (Clip* _clip)
    : clip (_clip)
{
    jassert (clip);
    clip->getState().addListener (this);
}

ArrangementViewTrackLaneClip::~ArrangementViewTrackLaneClip()
{
    clip->getState().removeListener (this);
}

void ArrangementViewTrackLaneClip::paint (Graphics&)
{
}

void ArrangementViewTrackLaneClip::valueTreePropertyChanged (ValueTree&, const Identifier& property)
{
    if (property == clip->start.getPropertyID())
    {
        int newXPos = getXPosForBeat (clip->start.get());
        setBounds (getBounds().withLeft (newXPos));
    }
    else if (property == clip->length.getPropertyID())
    {
        int newXPos = getXPosForBeat (clip->start.get() + clip->length.get());
        setBounds (getBounds().withRight (newXPos));
    }
}
