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

MidiFile interchange::callPython(MidiFile song) {
    std::string file = convert(song);
    py::scoped_interpreter guard{};
    py::bytes bytes(file);
    py::module python = py::module::import("python");
    py::object result = python.attr("openFile")(bytes);
    std::string n = result.cast<std::string>();
    MemoryBlock block(n.data(), n.size());
    MidiFile newSong = MidiFile();
    MemoryInputStream inputStream(block.getData(), block.getSize(), false);
    newSong.readFrom(inputStream);
    return newSong;
}

std::string interchange::convert(MidiFile song) {
    MemoryBlock file = MemoryBlock();
    MemoryOutputStream stream(file, false);
    song.writeTo(stream);
    std::string s;
    s.resize(file.getSize());
    memcpy(&s[0], file.getData(), file.getSize());
    return s;
}
