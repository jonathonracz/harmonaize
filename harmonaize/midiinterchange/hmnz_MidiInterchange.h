/*
  ==============================================================================

    hmnz_MidiInterchange.h
    Created: 6 Feb 2018 1:04:36pm
    Author:  Landon

  ==============================================================================
*/

#pragma once

#include "InterchangeTest/JuceLibraryCode/JuceHeader.h"
#include "../external/pybind11/include/pybind11/stl.h"
#include "../external/pybind11/include/pybind11/embed.h"
#include <iostream>
#include <vector>
#include <string>

namespace py = pybind11;
using namespace py::literals;

class interchange {
public:
    MidiFile callPython(std::string file);
private:
    std::string convert(std::string file);
    MidiFile getValue(void* value);
};
