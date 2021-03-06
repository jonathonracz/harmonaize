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

template<>
struct VariantConverter<Identifier>
{
    static Identifier fromVar (const var& v)
    {
        return Identifier (v);
    }

    static var toVar (const Identifier& i)
    {
        return i.toString();
    }
};

template<>
struct VariantConverter<MemoryBlock>
{
    static MemoryBlock fromVar (const var& v)
    {
        return *v.getBinaryData();
    }

    static var toVar (const MemoryBlock& m)
    {
        return var (m);
    }
};
