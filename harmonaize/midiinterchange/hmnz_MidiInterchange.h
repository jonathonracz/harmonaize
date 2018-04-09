/*
  ==============================================================================

    hmnz_MidiInterchange.h
    Created: 6 Feb 2018 1:04:36pm
    Author:  Landon

  ==============================================================================
*/

#pragma once

#if JUCE_CLANG
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wrange-loop-analysis"
#endif
#include "../external/pybind11/include/pybind11/stl.h"
#include "../external/pybind11/include/pybind11/embed.h"
#if JUCE_CLANG
#pragma clang diagnostic pop
#endif

#include <iostream>
#include <vector>
#include <string>

namespace py = pybind11;
using namespace py::literals;

class Interchange
{
public:
    static MidiFile callPython (MidiFile song) noexcept
    {
        File currentFile = File::getSpecialLocation (File::SpecialLocationType::currentApplicationFile);
        String path = currentFile.getFullPathName();
        std::string file = convert (song);
        py::scoped_interpreter guard{};
        py::module os = py::module::import ("os");
//        os.attr ("chdir")(path);
        std::string run = "import os\nos.chdir('" + path.toStdString() + "/Contents/Resources')";
        py::exec (run.c_str());
        py::bytes bytes (file);
        py::module python = py::module::import ("python");
        py::object result = python.attr ("openFile")(bytes);
        if (result.is_none())
        {
            return MidiFile();
        }
        std::string n = result.cast<std::string>();
        MemoryBlock block (n.data(), n.size());
        MidiFile newSong = MidiFile();
        MemoryInputStream is (block, false);
        newSong.readFrom (is);
        return newSong;
    }

private:
    static std::string convert (MidiFile song) noexcept
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
