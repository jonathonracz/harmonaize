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

    MidiMessageSequence getMidiMessageSequence (double timeDelta = 0.0) const noexcept
    {
        return getPartialMidiMessageSequence (std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max(), timeDelta);
    }

    MidiMessageSequence getPartialMidiMessageSequence (double rangeStart, double rangeEnd, double timeDelta = 0.0) const noexcept
    {
        auto begin = midiMessages.begin();
        auto end = midiMessages.end();

        MessageSortComparator sortComparator;
        auto startIt = std::lower_bound (begin, end, rangeStart, sortComparator);
        auto endIt = std::upper_bound (begin, end, rangeEnd, sortComparator);
        MidiMessageSequence ret;
        for (auto it = startIt; it != endIt; ++it)
            ret.addEvent (it->getMessage(), timeDelta);

        return ret;
    }

    void setMidiMessageSequence (const MidiMessageSequence& sequence, double timeDelta = 0.0) noexcept
    {
        getState().removeAllChildren (getUndoManager());
        addMidiMessageSequence (sequence, timeDelta);
    }

    void addMidiMessageSequence (const MidiMessageSequence& sequence, double timeDelta = 0.0) noexcept
    {
        for (MidiMessageSequence::MidiEventHolder* event : sequence)
             addEvent (event->message, timeDelta);
    }

    void addEvent (const MidiMessage& message, double timeDelta = 0.0) noexcept
    {
        MessageSortComparator sortComparator;
        auto insertLocation =
            std::upper_bound (midiMessages.begin(),
                              midiMessages.end(),
                              message.getTimeStamp(),
                              sortComparator);

        int insertIndex = (insertLocation == midiMessages.end()) ? -1 : midiMessages.indexOf (*insertLocation);
        getState().addChild (MidiMessageModel::createStateForMessage (message, timeDelta), insertIndex, getUndoManager());
    }

    void addTimeToMessages (double timeDelta) noexcept
    {
        for (MidiMessageModel* message : midiMessages)
            message->timestamp = message->timestamp.get() + timeDelta;
    }

    MidiMessageModel* findNoteOnOffSiblingFor (MidiMessageModel* message) const noexcept
    {
        int indexOfMessage = midiMessages.indexOf (message);
        if (!message || indexOfMessage < 0)
            return nullptr;

        MidiMessage messageObject = message->getMessage();
        if (messageObject.isNoteOn (true))
        {
            for (int i = indexOfMessage + 1; i < midiMessages.size(); ++i)
            {
                MidiMessage potentialPartnerMessage = midiMessages[i]->getMessage();
                if (potentialPartnerMessage.isNoteOff (true) && potentialPartnerMessage.getNoteNumber() == messageObject.getNoteNumber())
                    return midiMessages[i];
            }
        }
        else if (messageObject.isNoteOff (true))
        {
            for (int i = indexOfMessage - 1; i > 0; --i)
            {
                MidiMessage potentialPartnerMessage = midiMessages[i]->getMessage();
                if (potentialPartnerMessage.isNoteOn (true) && potentialPartnerMessage.getNoteNumber() == messageObject.getNoteNumber())
                    return midiMessages[i];
            }
        }

        return nullptr;
    }

    void trimMessagesOutsideBounds (double start, double end) noexcept
    {
        Array<MidiMessageModel*> messagesToRemove;
        Array<MidiMessage> messagesToAdd;
        for (MidiMessageModel* message : midiMessages)
        {
            if (message->timestamp < start || message->timestamp > end)
            {
                MidiMessage messageObject = message->getMessage();
                if (messageObject.isNoteOnOrOff())
                {
                    MidiMessageModel* sibling = findNoteOnOffSiblingFor (message);
                    if (sibling)
                    {
                        if (messageObject.isNoteOn (true))
                        {
                            jassert (sibling->getMessage().isNoteOff (true));
                            if (sibling->timestamp > start)
                            {
                                messagesToRemove.add (message);
                                messagesToAdd.add (MidiMessage (messageObject, start));
                            }
                        }
                        else
                        {
                            jassert (sibling->getMessage().isNoteOn (true));
                            if (sibling->timestamp < end)
                            {
                                messagesToRemove.add (message);
                                messagesToAdd.add (MidiMessage (messageObject, end));
                            }
                        }
                    }
                }

                messagesToRemove.add (message);
            }
        }

        for (MidiMessageModel* message : messagesToRemove)
            midiMessages.removeObject (message);

        for (MidiMessage& message : messagesToAdd)
            addEvent (message);
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
