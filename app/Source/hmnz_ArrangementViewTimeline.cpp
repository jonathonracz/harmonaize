/*
  ==============================================================================

    hmnz_ArrangementViewTimeline.cpp
    Created: 7 Mar 2018 1:09:19pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_ArrangementViewTimeline.h"
#include "hmnz_Edit.h"

void ArrangementViewTimeline::resized() noexcept
{
}

void ArrangementViewTimeline::paint (Graphics& g) noexcept
{
    if (!getEdit())
        return;

    ArrangementViewModel& model = getEdit()->arrangementViewModel;
    NormalisableRange<double> remapper = getBeatRemapper();
    const int minimumLineSpacing = 14.0; // Arbitrary - controls how close lines can get before the grid size increases
    double linesPerBeat = getLinesPerBeatForMinimumLineSpacing (minimumLineSpacing);

    double beatValue = Utility::floorToNearestInterval (remapper.start, linesPerBeat);
    while (beatValue <= remapper.end)
    {
        TimeSignature& timeSignature = getEdit()->masterTrack.timeSignature;
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

    double transportBeat = getEdit()->transport.playHeadBeat;
    if (transportBeat >= model.timeStart)
    {
        g.setColour (Colours::blue);
        int transportPos = getXPosForBeat (transportBeat);
        lastPaintedPlayHeadBounds = Rectangle<int> (transportPos, 0, 2, getHeight());
        g.fillRect (lastPaintedPlayHeadBounds);
    }
}

void ArrangementViewTimeline::valueTreePropertyChanged (ValueTree& tree, const Identifier& property)
{
    if (tree == getEdit()->transport.getState())
    {
        if (property == getEdit()->transport.playHeadBeat.getPropertyID())
        {
            Rectangle<int> expectedPlayHeadBounds = Rectangle<int> (getXPosForBeat (getEdit()->transport.playHeadBeat.get()), 0, 2, getHeight());
            repaint (expectedPlayHeadBounds.getUnion (lastPaintedPlayHeadBounds));
        }
    }
}
