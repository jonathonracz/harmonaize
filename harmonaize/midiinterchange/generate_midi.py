#!/usr/bin/env python 

import os
import sys
import mido
from parse_midi import parseMidi
from chord_prog import ChordProg
from grooves import GROOVES

def genAccompaniment(midi=None):
	print(sys.version)
	FileAttributes = parseMidi(midi)
	generator = ChordProg(FileAttributes['key'], "generated_files/accomp.mma", FileAttributes['tempo'], FileAttributes['groove'])
	generator.generateMMAFormat()

	genMidi("generated_files/accomp.mma")

	return mido.MidiFile("generated_files/accomp.mid")

def genMidi(path_to_fakebook):
	os.system('python3 --version')
	os.system('../external/mma/mma.py -g && python ../external/mma/mma.py ' + path_to_fakebook)

if __name__ == '__main__':
	midi_file = mido.MidiFile('../../app/Design/test.mid')
	genAccompaniment(midi_file)
