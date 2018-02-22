/*
  ==============================================================================

    hmnz_MidiInterchange.h
    Created: 6 Feb 2018 1:04:36pm
    Author:  Landon

  ==============================================================================
*/

#pragma once

#include "../external/pybind11/include/pybind11/stl.h"
#include "../external/pybind11/include/pybind11/embed.h"
#include <iostream>
#include <vector>
#include <string>

namespace py = pybind11;
using namespace py::literals;

class Interchange {
public:
    static MidiFile callPython (MidiFile file) noexcept
    {
        std::string file = convert (song);
        py::scoped_interpreter guard{};
        py::bytes bytes (file);
        py::module python = py::module::import ("python");
        py::object result = python.attr ("openFile")(bytes);
        std::string n = result.cast<std::string>();
        //std::cout << std::endl << n << std::endl;
        MemoryBlock block (n.data(), n.size());
        MidiFile newSong = MidiFile();
        File f = File ("./generated_files/accomp.mid");
        FileInputStream is (f);
        newSong.readFrom (is);
        return newSong;
    }

private:
    static std::string convert (MidiFile file) noexcept
    {
        MemoryBlock file = MemoryBlock();
        MemoryOutputStream stream (file, false);
        song.writeTo (stream);
        std::string s;
        s.resize (file.getSize());
        memcpy (&s[0], file.getData(), file.getSize());
        return s;
    }
};
