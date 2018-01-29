/*
  ==============================================================================

    hmnz_VariantConverters.h
    Created: 29 Jan 2018 1:20:58pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

template<>
struct VariantConverter<Colour>
{
    static Colour fromVar (const var& v)
    {
        return Colour::fromString (v.toString());
    }

    static var toVar (const Colour& c)
    {
        return c.toString();
    }
};
