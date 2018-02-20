/*
  ==============================================================================

    hmnz_MidiMessageSequenceModel.h
    Created: 16 Feb 2018 2:41:09pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_MidiMessageModel.h"
#include "hmnz_GenericValueTreeObjectArray.h"
#include "hmnz_ArrayIterator.h"

class MidiMessageSequenceModel  : public ValueTreeObject<IDs::MidiMessageSequenceModel>
{
public:
    MidiMessageSequenceModel (const ValueTree& v, UndoManager* um)
        : ValueTreeObject (v, um), midiMessages (getState(), getUndoManager())
    {
    }

    MidiMessageSequence getMidiMessageSequence() const noexcept
    {
        return getPartialMidiMessageSequence (std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max());
    }

    MidiMessageSequence getPartialMidiMessageSequence (double rangeStart, double rangeEnd) const noexcept
    {
        jdr::ArrayForwardIterator<MidiMessageModel*> begin = jdr::ArrayForwardIterator<MidiMessageModel*>::begin (midiMessages.objects);
        jdr::ArrayForwardIterator<MidiMessageModel*> end = jdr::ArrayForwardIterator<MidiMessageModel*>::end (midiMessages.objects);

        MessageSortComparator sortComparator;
        jdr::ArrayForwardIterator<MidiMessageModel*> startIt = std::lower_bound (begin, end, rangeStart, sortComparator);
        jdr::ArrayForwardIterator<MidiMessageModel*> endIt = std::upper_bound (begin, end, rangeEnd, sortComparator);
        MidiMessageSequence ret;
        for (jdr::ArrayForwardIterator<MidiMessageModel*> it = startIt; startIt != endIt; ++it)
            ret.addEvent (it->getMessage());

        return ret;
    }

    void setMidiMessageSequence (MidiMessageSequence& sequence) noexcept
    {
        getState().removeAllChildren (getUndoManager());
        addMidiMessageSequence (sequence);
    }

    void addMidiMessageSequence (MidiMessageSequence& sequence) noexcept
    {
        sequence.sort();
        for (MidiMessageSequence::MidiEventHolder* event : sequence)
             addEvent (event->message);
    }

    void addEvent (const MidiMessage& message) noexcept
    {
        MessageSortComparator sortComparator;
        jdr::ArrayForwardIterator<MidiMessageModel*> insertLocation =
            std::upper_bound (jdr::ArrayForwardIterator<MidiMessageModel*>::begin (midiMessages.objects),
                              jdr::ArrayForwardIterator<MidiMessageModel*>::end (midiMessages.objects),
                              message.getTimeStamp(),
                              sortComparator);

        int insertIndex = (insertLocation == jdr::ArrayForwardIterator<MidiMessageModel*>::end (midiMessages.objects)) ? -1 : midiMessages.objects.indexOf (*insertLocation);
        getState().addChild (MidiMessageModel::createStateForMessage (message), insertIndex, getUndoManager());
    }

    GenericValueTreeObjectArray<MidiMessageModel> midiMessages;

private:
    struct MessageSortComparator
    {
        bool operator() (MidiMessageModel* first, MidiMessageModel* second)
        {
            return first->timestamp < second->timestamp;
        }

        bool operator() (double first, MidiMessageModel* second)
        {
            return first < second->timestamp;
        }

        bool operator() (MidiMessageModel* first, double second)
        {
            return first->timestamp < second;
        }
    };
};
