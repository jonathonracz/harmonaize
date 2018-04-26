/*
  ==============================================================================

    Genre.h
    Created: 12 Apr 2018 12:14:38pm
    Author:  Marc Lashner

  ==============================================================================
*/

#pragma once

#include "hmnz_ValueTreeObject.h"

class Genre : public ValueTreeObject<IDs::Genre>
{
public:
    Genre (const ValueTree& v, UndoManager* um)
        : ValueTreeObject (v, um), genre (getState(), IDs::GenreProps::GenreIndex, getUndoManager(), 0)
    {
    }

    static StringArray getGenres()
    {
        StringArray ret;
        ret.add (translate ("Jazz"));
        ret.add (translate ("Swing"));
        ret.add (translate ("Pop"));
        ret.add (translate ("Rock"));
        ret.add (translate ("Folk"));
        ret.add (translate ("Ballad"));
        ret.add (translate ("Random"));
        return ret;
    }

    static int getIndexFromString (const String& genre)
    {
        StringArray genres = getGenres();
        for (int i = 0; i < 7; ++i)
        {
            if (genres[i] == genre)
                return i;
        }
        return -1;
    }

    static String getStringFromIndex (int index)
    {
        StringArray genres = getGenres();
        return genres[index];
    }

    CachedValue<int> genre;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Genre)
};
