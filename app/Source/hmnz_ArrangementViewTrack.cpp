/*
  ==============================================================================

    hmnz_ArrangementViewTrack.cpp
    Created: 14 Mar 2018 1:48:47pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_ArrangementViewTrack.h"
#include "hmnz_Edit.h"

ArrangementViewTrack::ArrangementViewTrack (Track& representedTrack)
    : track (representedTrack), header (representedTrack),
      lane (representedTrack), heightResizer (representedTrack)
{
    track.edit.arrangementViewModel.getState().addListener (this);
    addAndMakeVisible (header);
    addAndMakeVisible (lane);
    addAndMakeVisible (heightResizer);
}

ArrangementViewTrack::~ArrangementViewTrack()
{
    track.edit.arrangementViewModel.getState().removeListener (this);
}

void ArrangementViewTrack::resized()
{
    FlexBox layout;
    layout.flexDirection = FlexBox::Direction::column;

    FlexBox flexBox;
    flexBox.flexDirection = FlexBox::Direction::row;
    flexBox.items.add (FlexItem (lane).withFlex (1.0f));
    flexBox.items.add (FlexItem (header).withWidth (static_cast<int> (track.edit.arrangementViewModel.headerWidth.get())));

    layout.items.add (FlexItem (flexBox).withFlex (1.0f));
    layout.items.add (FlexItem (heightResizer).withHeight (2));

    layout.performLayout (getLocalBounds());
}

void ArrangementViewTrack::valueTreePropertyChanged (ValueTree&, const Identifier& property)
{
    if (property == track.edit.arrangementViewModel.headerWidth.getPropertyID())
    {
        resized();
        repaint();
    }
}
