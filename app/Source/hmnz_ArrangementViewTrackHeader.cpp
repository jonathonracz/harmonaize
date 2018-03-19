/*
  ==============================================================================

    hmnz_ArrangementViewTrackHeader.cpp
    Created: 7 Mar 2018 4:47:06pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_ArrangementViewTrackHeader.h"
#include "hmnz_Track.h"

ArrangementViewTrackHeader::ArrangementViewTrackHeader (Track& _track)
    : track (_track)
{
    addAndMakeVisible (name);
    name.addListener (this);
    name.setEditable (true, true, false);
    track.getState().addListener (this);
    track.getState().sendPropertyChangeMessage (track.name.getPropertyID());
}

ArrangementViewTrackHeader::~ArrangementViewTrackHeader()
{
    track.getState().removeListener (this);
}

void ArrangementViewTrackHeader::resized()
{
    name.setBounds (getLocalBounds());
}

void ArrangementViewTrackHeader::paint (Graphics& g)
{
    g.fillAll (track.color);
}

void ArrangementViewTrackHeader::valueTreePropertyChanged (ValueTree&, const Identifier& property)
{
    if (property == track.name.getPropertyID())
    {
        name.setText (track.name.get(), dontSendNotification);
    }
}

void ArrangementViewTrackHeader::editorHidden (Label *labelThatHasChanged, TextEditor& textEditor)
{
    track.name = textEditor.getText();
}
