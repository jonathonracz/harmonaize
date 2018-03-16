/*
  ==============================================================================

    hmnz_Clip.h
    Created: 22 Jan 2018 12:57:58am
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"
#include "hmnz_MidiMessageSequenceModel.h"

class Track;

class Clip  : public ValueTreeObject<IDs::Clip>
{
public:
    Clip (const ValueTree& v, UndoManager* um, Track& track);

    MidiMessageSequence getMidiMessageSequence (double timeDelta = 0.0) const;

    MidiMessageSequence getPartialMidiMessageSequence (double rangeStart, double rangeEnd, double timeDelta = 0.0) const;

    void setMidiMessageSequence (const MidiMessageSequence& sequence, double timeDelta = 0.0, bool resizeToFit = true);

    void addMidiMessageSequence (const MidiMessageSequence& sequence, double timeDelta = 0.0, bool resizeToFit = true);

    void addEvent (const MidiMessage& message, double timeDelta = 0.0, bool resizeToFit = true);

    void setStartTimeKeepingEndTime (double newStartTime);

    static Identifier defaultClipTypeForTrackType (const Identifier& trackType);

    static ValueTree createState (double start, double length, const Identifier& type, const Colour& color);

    static ValueTree createState (double start, double length, const MidiMessageSequence& sequence, const Colour& color);

    CachedValue<double> start;
    CachedValue<double> length;
    CachedValue<Colour> color;
    CachedValue<Identifier> type;

    MidiMessageSequenceModel midiMessageSequenceModel;
    Track& track;

private:
    void adjustBoundsToFitMessageTimestamp (double timestamp);

    JUCE_DECLARE_WEAK_REFERENCEABLE (Clip)
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Clip)
};
