/*
  ==============================================================================

    hmnz_MidiInterchange.cpp
    Created: 6 Feb 2018 1:04:36pm
    Author:  Landon

  ==============================================================================
*/

#include "hmnz_MidiInterchange.h"

namespace py = pybind11;
using namespace py::literals;

MidiFile interchange::getValue(void* value) {
    MidiFile* file = (MidiFile*)value;
    return *file;
}

MidiFile interchange::callPython(std::string file) {
    py::scoped_interpreter guard{};
    py::bytes bytes(file);
    py::module python = py::module::import("python");
    py::object result = python.attr("openFile")(bytes);
    std::string n = result.cast<std::string>();
    std::cout << n << std::endl;
    //    MidiFile f = getValue(nullptr);
}
