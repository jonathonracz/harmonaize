/*
  ==============================================================================

    hmnz_UnitTests.cpp
    Created: 28 Feb 2018 4:02:41pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#include "JuceHeader.h"

#if JUCE_DEBUG

#include "hmnz_ValueTreeObjectUnitTest.h"

static ValueTreeObjectUnitTest valueTreeObjectUnitTest;

#include "hmnz_ValueTreeObjectArrayUnitTest.h"

static ValueTreeObjectArrayUnitTest valueTreeObjectArrayUnitTest;

#include "hmnz_ValueTreeObjectWithArrayUnitTest.h"

static ValueTreeObjectWithArrayUnitTest valueTreeObjectWithArrayUnitTest;

#endif
