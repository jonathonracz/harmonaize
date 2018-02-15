#!/usr/bin/env python 

import os
import sys
from parse_midi import parse_midi
from chord_prog import ChordProg
from grooves import GROOVES

def genAccompaniment(midi=None):
	FileAttributes = parse_midi(midi)
	generator = ChordProg(FileAttributes.key, "generated_files/accomp.mma", FileAttributes.tempo, FileAttributes.groove)
	generator.generateMMAFormat()

	genMidi("generated_files/accomp.mma")

	return mido.MidiFile("generated_files/accomp.mma")

def genMidi(path_to_fakebook):
	os.system('python3 ../external/mma/mma.py ' + path_to_fakebook)

if __name__ == '__main__':
    genFakebook(*sys.argv[1:])
