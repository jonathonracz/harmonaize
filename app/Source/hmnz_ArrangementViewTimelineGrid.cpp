/*
  ==============================================================================

    hmnz_ArrangementViewTimelineGrid.cpp
    Created: 7 Mar 2018 1:09:19pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_ArrangementViewTimelineGrid.h"
#include "hmnz_Edit.h"

ArrangementViewTimelineGrid::ArrangementViewTimelineGrid (Edit& edit)
    : ArrangementViewTimelineComponent (edit)
{
    edit.transport.getState().addListener (this);
    edit.arrangementViewModel.getState().addListener (this);
}

ArrangementViewTimelineGrid::~ArrangementViewTimelineGrid()
{
    edit.transport.getState().removeListener (this);
    edit.arrangementViewModel.getState().removeListener (this);
}

void ArrangementViewTimelineGrid::paint (Graphics& g)
{
    const ArrangementViewModel& model = edit.arrangementViewModel;
    NormalisableRange<double> remapper = getBeatRemapper();
    const int minimumLineSpacing = 14.0; // Arbitrary - controls how close lines can get before the grid size increases
    double linesPerBeat = getLinesPerBeatForMinimumLineSpacing (minimumLineSpacing);

    double beatValue = Utility::floorToNearestInterval (remapper.start, linesPerBeat);
    while (beatValue <= remapper.end)
    {
        const TimeSignature& timeSignature = edit.masterTrack.timeSignature;
        const double beatsInBar = timeSignature.getNumeratorAtBeat (beatValue);
        const double bar = timeSignature.barForBeat (beatValue);
        const double beat = std::floor (timeSignature.beatInBar (beatValue));
        const double subBeat = std::fmod (beatValue, 1.0);

        Colour color = (beat == 0.0 && subBeat == 0.0) ? Colours::white : Colours::grey;
        g.setColour (color);

        int xPos = getXPosForBeat (beatValue);
        g.fillRect (Rectangle<int> (xPos, 0, 1, getHeight()));
        beatValue += linesPerBeat;
    }
}

void ArrangementViewTimelineGrid::valueTreePropertyChanged (ValueTree& tree, const Identifier& property)
{
    if (property == edit.arrangementViewModel.timeStart.getPropertyID() ||
             property == edit.arrangementViewModel.timeEnd.getPropertyID())
    {
        repaint();
    }
}
