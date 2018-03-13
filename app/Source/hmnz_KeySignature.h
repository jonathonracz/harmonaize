/*
  ==============================================================================

    hmnz_KeySignature.h
    Created: 12 Feb 2018 11:06:14pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"

class KeySignature  : public ValueTreeObject<IDs::KeySignature>
{
public:
    struct Snapshot
    {
        int numSharpsOrFlats;
        bool isMinor;
        double snapshotTime;
    };

    KeySignature (const ValueTree& v, UndoManager* um)
        : ValueTreeObject (v, um),
          numSharpsOrFlats (getState(), IDs::KeySignatureProps::NumberOfSharpsOrFlats, getUndoManager(), 0),
          isMinor (getState(), IDs::KeySignatureProps::IsMinor, getUndoManager(), false)
    {
    }

    static StringArray getKeyDescriptions()
    {
        StringArray ret;
        for (int majorMinor = 0; majorMinor < 2; ++majorMinor)
            for (int key = -7; key < 8; ++key)
                ret.add (createKeyDescription (key, static_cast<bool> (majorMinor)));

        return ret;
    }

    static String createKeyDescription (int numSharpsOrFlats, bool isMinor)
    {
        jassert (numSharpsOrFlats >= 7 || numSharpsOrFlats <= 7);
        String ret;
        if (!isMinor)
        {
            switch (numSharpsOrFlats)
            {
                case -7: ret = CharPointer_UTF8 ("C\u266d"); break;
                case -6: ret = CharPointer_UTF8 ("G\u266d"); break;
                case -5: ret = CharPointer_UTF8 ("D\u266d"); break;
                case -4: ret = CharPointer_UTF8 ("A\u266d"); break;
                case -3: ret = CharPointer_UTF8 ("E\u266d"); break;
                case -2: ret = CharPointer_UTF8 ("B\u266d"); break;
                case -1: ret = "F"; break;
                case 0: ret = "C"; break;
                case 1: ret = "G"; break;
                case 2: ret = "D"; break;
                case 3: ret = "A"; break;
                case 4: ret = "E"; break;
                case 5: ret = "B"; break;
                case 6: ret = CharPointer_UTF8 ("F\u266f"); break;
                case 7: ret = CharPointer_UTF8 ("C\u266f"); break;
                default: jassertfalse;
            }
            ret += " major";
        }
        else
        {
            switch (numSharpsOrFlats)
            {
                case -7: ret = CharPointer_UTF8 ("A\u266f"); break;
                case -6: ret = CharPointer_UTF8 ("E\u266f"); break;
                case -5: ret = CharPointer_UTF8 ("B\u266f"); break;
                case -4: ret = "F"; break;
                case -3: ret = "C"; break;
                case -2: ret = "G"; break;
                case -1: ret = "D"; break;
                case 0: ret = "A"; break;
                case 1: ret = "E"; break;
                case 2: ret = "B"; break;
                case 3: ret = CharPointer_UTF8 ("F\u266d"); break;
                case 4: ret = CharPointer_UTF8 ("C\u266d"); break;
                case 5: ret = CharPointer_UTF8 ("G\u266d"); break;
                case 6: ret = CharPointer_UTF8 ("D\u266d"); break;
                case 7: ret = CharPointer_UTF8 ("A\u266d"); break;
                default: jassertfalse;
            }
            ret += " minor";
        }

        return ret;
    }

    static std::pair<int, bool> createRepresentationFromDescription (const String& description)
    {
        jassert (description.containsWholeWordIgnoreCase ("major") || description.containsWholeWordIgnoreCase ("minor"));
        bool isMinor = description.containsWholeWordIgnoreCase ("minor");
        int numSharpsOrFlats = 0;
        if (!isMinor)
        {
            if (description.startsWithIgnoreCase (CharPointer_UTF8 ("C\u266d"))) numSharpsOrFlats = -7;
            else if (description.startsWithIgnoreCase (CharPointer_UTF8 ("G\u266d"))) numSharpsOrFlats = -6;
            else if (description.startsWithIgnoreCase (CharPointer_UTF8 ("D\u266d"))) numSharpsOrFlats = -5;
            else if (description.startsWithIgnoreCase (CharPointer_UTF8 ("A\u266d"))) numSharpsOrFlats = -4;
            else if (description.startsWithIgnoreCase (CharPointer_UTF8 ("E\u266d"))) numSharpsOrFlats = -3;
            else if (description.startsWithIgnoreCase (CharPointer_UTF8 ("B\u266d"))) numSharpsOrFlats = -2;
            else if (description.startsWithIgnoreCase ("F")) numSharpsOrFlats = -1;
            else if (description.startsWithIgnoreCase ("C")) numSharpsOrFlats = 0;
            else if (description.startsWithIgnoreCase ("G")) numSharpsOrFlats = 1;
            else if (description.startsWithIgnoreCase ("D")) numSharpsOrFlats = 2;
            else if (description.startsWithIgnoreCase ("A")) numSharpsOrFlats = 3;
            else if (description.startsWithIgnoreCase ("E")) numSharpsOrFlats = 4;
            else if (description.startsWithIgnoreCase ("B")) numSharpsOrFlats = 5;
            else if (description.startsWithIgnoreCase (CharPointer_UTF8 ("F\u266f"))) numSharpsOrFlats = 6;
            else if (description.startsWithIgnoreCase (CharPointer_UTF8 ("C\u266f"))) numSharpsOrFlats = 7;
            else jassertfalse;
        }
        else
        {
            if (description.startsWithIgnoreCase (CharPointer_UTF8 ("A\u266f"))) numSharpsOrFlats = -7;
            else if (description.startsWithIgnoreCase (CharPointer_UTF8 ("E\u266f"))) numSharpsOrFlats = -6;
            else if (description.startsWithIgnoreCase (CharPointer_UTF8 ("B\u266f"))) numSharpsOrFlats = -5;
            else if (description.startsWithIgnoreCase ("F")) numSharpsOrFlats = -4;
            else if (description.startsWithIgnoreCase ("C")) numSharpsOrFlats = -3;
            else if (description.startsWithIgnoreCase ("G")) numSharpsOrFlats = -2;
            else if (description.startsWithIgnoreCase ("D")) numSharpsOrFlats = -1;
            else if (description.startsWithIgnoreCase ("A")) numSharpsOrFlats = 0;
            else if (description.startsWithIgnoreCase ("E")) numSharpsOrFlats = 1;
            else if (description.startsWithIgnoreCase ("B")) numSharpsOrFlats = 2;
            else if (description.startsWithIgnoreCase (CharPointer_UTF8 ("F\u266d"))) numSharpsOrFlats = 3;
            else if (description.startsWithIgnoreCase (CharPointer_UTF8 ("C\u266d"))) numSharpsOrFlats = 4;
            else if (description.startsWithIgnoreCase (CharPointer_UTF8 ("G\u266d"))) numSharpsOrFlats = 5;
            else if (description.startsWithIgnoreCase (CharPointer_UTF8 ("D\u266d"))) numSharpsOrFlats = 6;
            else if (description.startsWithIgnoreCase (CharPointer_UTF8 ("A\u266d"))) numSharpsOrFlats = 7;
            else jassertfalse;
        }

        return std::make_pair (numSharpsOrFlats, isMinor);
    }

    Snapshot getKeySignatureAtTime (double time) const
    {
        Snapshot ret;
        ret.numSharpsOrFlats = numSharpsOrFlats.get().getRelaxed();
        ret.isMinor = isMinor.get().getRelaxed();
        std::atomic_thread_fence (std::memory_order_acquire);
        ret.snapshotTime = time;
        return ret;
    }

    void setKeySignatureAtTime (Snapshot keySignature)
    {
        std::atomic_thread_fence (std::memory_order_acquire);
        numSharpsOrFlats.get().setRelaxed (keySignature.numSharpsOrFlats);
        isMinor.get().setRelaxed (keySignature.isMinor);
    }

    int getNumSharpsOrFlatsAtTime (double time) const
    {
        return numSharpsOrFlats.get();
    }

    void setNumSharpsOrFlatsAtTime (int newNumSharpsOrFlats, double time)
    {
        return numSharpsOrFlats.setValue (newNumSharpsOrFlats, getUndoManager());
    }

    int getIsMinorAtTime (double time) const
    {
        return isMinor.get();
    }

    void setIsMinorAtTime (int newIsMinor, double time)
    {
        return isMinor.setValue (newIsMinor, getUndoManager());
    }

private:
    CachedValue<SPSCAtomicWrapper<int>> numSharpsOrFlats;
    CachedValue<SPSCAtomicWrapper<bool>> isMinor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeySignature)
};
