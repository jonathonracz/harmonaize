/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

#include <iostream>
#include "../../hmnz_MidiInterchange.h"
#include <string>


//==============================================================================
int main (int argc, char* argv[])
{
    MemoryBlock clocksMemory(BinaryData::Skyfall_mid, BinaryData::Skyfall_midSize);
    MidiFile clocks = MidiFile();
    MemoryInputStream inputStream(clocksMemory.getData(), clocksMemory.getSize(), false);
    clocks.readFrom(inputStream);
//    std::cout << BinaryData::clocks_midSize << " " << clocksMemory.getSize() << std::endl;
    std::string s;
    s.resize(clocksMemory.getSize());
    memcpy(&s[0], clocksMemory.getData(), clocksMemory.getSize());
//    std::cout << s.size() << std::endl;
    interchange ic = interchange();
    MidiFile result = ic.callPython(clocks);
    return 0;
}
