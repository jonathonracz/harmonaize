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

class MidiMessageSequenceModel  : ValueTreeObject<IDs::MidiMessageSequenceModel>
{
public:
    MidiMessageSequenceModel (const ValueTree& v, UndoManager* um)
        : ValueTreeObject (v, um), midiMessages (getState(), getUndoManager())
    {
    }

    MidiMessageSequence getMidiMessageSequence() const noexcept
    {
        MidiMessageSequence ret;
        for (MidiMessageModel* midiMessage : midiMessages.objects)
            ret.addEvent (midiMessage->getMessage());

        return ret;
    }

    void setMidiMessageSequence (const MidiMessageSequence& sequence) noexcept
    {
        getState().removeAllChildren (getUndoManager());
        for (MidiMessageSequence::MidiEventHolder* event : sequence)
            addEvent (event->message);
    }

    void addEvent (const MidiMessage& message) noexcept
    {
        jdr::ArrayForwardIterator<MidiMessageModel*> insertLocation =
            std::upper_bound (jdr::ArrayForwardIterator<MidiMessageModel*>::begin (midiMessages.objects),
                              jdr::ArrayForwardIterator<MidiMessageModel*>::end (midiMessages.objects),
                              message.getTimeStamp(),
                              [&](double first, MidiMessageModel* second) {
                                  return first < second->timestamp;
                              });

        int insertIndex = (insertLocation == jdr::ArrayForwardIterator<MidiMessageModel*>::end (midiMessages.objects)) ? -1 : midiMessages.objects.indexOf (*insertLocation);
        getState().addChild (MidiMessageModel::createStateForMessage (message), insertIndex, getUndoManager());
    }

private:
    GenericValueTreeObjectArray<MidiMessageModel> midiMessages;
};
