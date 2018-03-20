/*
  ==============================================================================

    hmnz_ArrangementViewTrackHeader.cpp
    Created: 7 Mar 2018 4:47:06pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_ArrangementViewTrackHeader.h"
#include "hmnz_Track.h"

struct ArrangementViewTrackHeader::ProgressBarBackground : public LookAndFeel_V4
{
    void drawProgressBar (Graphics & g, ProgressBar &, int width, int height, double progress, const String &textToShow) override
    {
        g.fillAll (findColour (ProgressBar::ColourIds::backgroundColourId));
        g.setColour (findColour (ProgressBar::ColourIds::foregroundColourId));
        g.fillRect (0, 0, std::abs (static_cast<int> (width * progress)), height);
    }
};

ArrangementViewTrackHeader::ArrangementViewTrackHeader (Track& _track)
    : track (_track), loadProgress (*_track.loadFuture.progress)
{
    loadProgress.setPercentageDisplay (false);
    progressBarBackground = std::unique_ptr<ProgressBarBackground> (new ProgressBarBackground());
    progressBarBackground->setColour (ProgressBar::ColourIds::foregroundColourId, track.color);
    progressBarBackground->setColour (ProgressBar::ColourIds::backgroundColourId, Colours::transparentWhite);
    loadProgress.setLookAndFeel (progressBarBackground.get());

    addAndMakeVisible (loadProgress);
    addAndMakeVisible (name);

    name.addListener (this);
    name.setEditable (true, true, false);
    track.getState().addListener (this);
    track.getState().sendPropertyChangeMessage (track.name.getPropertyID());
}

ArrangementViewTrackHeader::~ArrangementViewTrackHeader()
{
    track.getState().removeListener (this);
    loadProgress.setLookAndFeel (nullptr);
}

void ArrangementViewTrackHeader::resized()
{
    loadProgress.setBounds (getLocalBounds());
    name.setBounds (getLocalBounds());
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
