/*
  ==============================================================================

    hmnz_ArrangementViewHeaderControls.cpp
    Created: 19 Mar 2018 6:11:10pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_ArrangementViewHeaderControls.h"
#include "hmnz_Edit.h"

ArrangementViewHeaderControls::ArrangementViewHeaderControls (Edit& _edit)
    : edit (_edit)
{
    addTrack.setButtonText (translate ("Add track"));
    addAndMakeVisible (addTrack);
    addTrack.addListener (this);
}

void ArrangementViewHeaderControls::resized()
{
    addTrack.setBounds (getLocalBounds());
}

void ArrangementViewHeaderControls::buttonClicked (Button*)
{
    edit.trackList.tracks.addState (Track::createDefaultState());
}
