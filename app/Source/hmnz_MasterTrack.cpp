/*
  ==============================================================================

    hmnz_MasterTrack.cpp
    Created: 30 Jan 2018 6:31:27pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_MasterTrack.h"
#include "hmnz_Edit.h"

MasterTrack::MasterTrack (Edit* const _edit)
    : ValueTreeObject (_edit->getState(), _edit->getUndoManager()),
      edit (_edit),
      beatsPerMinute (getState(), IDs::MasterTrackProps::BeatsPerMinute, nullptr, 120.0),
      timeSigNumerator (getState(), IDs::MasterTrackProps::TimeSigNumerator, nullptr, 4),
      timeSigDenominator (getState(), IDs::MasterTrackProps::TimeSigDenominator, nullptr, 4)
{
}

double MasterTrack::getBeatsPerMinuteAtTime (double time)
{
    return beatsPerMinute.get(); // No automation yet
}

double MasterTrack::getSecondsPerBeatAtTime (double time)
{

}

double MasterTrack::getBeatsAtTime (double time)
{

}
