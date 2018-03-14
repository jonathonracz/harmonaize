/*
  ==============================================================================

    hmnz_ClipArray.cpp
    Created: 11 Mar 2018 12:53:26pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "hmnz_ClipArray.h"
#include "hmnz_Track.h"

ClipArray::ClipArray (const ValueTree& v, UndoManager* um, Track& _track) :
    HomogeneousValueTreeObjectArray<Clip> (v, um), track (_track)
{

}

Clip* ClipArray::createNewObject (const ValueTree& v, UndoManager* um)
{
    return new Clip (v, um, track);
}
