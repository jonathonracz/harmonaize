/*
  ==============================================================================

    hmnz_ArrangementViewTimelineGrid.cpp
    Created: 7 Mar 2018 1:09:19pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_ArrangementViewTimelineGrid.h"
#include "hmnz_Edit.h"

ArrangementViewTimelineGrid::ArrangementViewTimelineGrid (Edit& _edit, CachedValue<double>& timeStart, CachedValue<double>& timeEnd)
    : ArrangementViewTimelineComponent (timeStart, timeEnd), edit (_edit)
{
    edit.masterTrack.timeSignature.getState().addListener (this);
    timeStart.getValueTree().addListener (this);
    timeEnd.getValueTree().addListener (this);
}

ArrangementViewTimelineGrid::~ArrangementViewTimelineGrid()
{
    edit.masterTrack.timeSignature.getState().removeListener (this);
    timeStart.getValueTree().removeListener (this);
    timeEnd.getValueTree().removeListener (this);
}

void ArrangementViewTimelineGrid::paint (Graphics& g)
{
    NormalisableRange<double> remapper = getBeatRemapper();
    const int minimumLineSpacing = 14; // TODO: Arbitrary - controls how close lines can get before the grid size increases
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
    if (tree == edit.masterTrack.getState() ||
        property == timeStart.getPropertyID() ||
        property == timeEnd.getPropertyID())
    {
        repaint();
    }
}
