/*
  ==============================================================================

    hmnz_Identifiers.h
    Created: 22 Jan 2018 3:17:40pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

namespace IDs
{
#define DECLARE_ID(name) const Identifier name (#name);

    DECLARE_ID (Edit)
    DECLARE_ID (Track)
    DECLARE_ID (Clip)
    DECLARE_ID (Note)

#undef DECLARE_ID
}
