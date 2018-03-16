#!/usr/bin/env python 

import os
import sys
import mido

from chord_prog import ChordProg
from midi_parser import MidiParser
from groove_selector import GrooveSelector

from grooves import GROOVES

def genAccompaniment(midi=None):
	parser = MidiParser(midi)

	note_counts, beat_map = parser.getMidiInfo()
	selector = GrooveSelector(parser.getTempo(), parser.getTimeSignature())

	FileAttributes = {
		'tempo': parser.getTempo(),
		'tonic': parser.getTonic(),
		'time_signature': parser.getTimeSignature(),
		'note_counts': note_counts,
		'beat_map': beat_map,
		'groove': selector.select_groove(),
	}

	generator = ChordProg(FileAttributes, "generated_files/accomp.mma")
	generator.generateMMAFormat()

	genMidi("generated_files/accomp.mma")
	return mido.MidiFile("../midiinterchange/generated_files/accomp.mid")

def genMultipleAccompaniments(midi=None):
	FileAttributes1 = parseMidi(midi)
	FileAttributes2 = parseMidi(midi)
	FileAttributes3 = parseMidi(midi)

	while FileAttributes2['groove'] == FileAttributes1['groove']:
		FileAttributes2 = parseMidi(midi)

	while FileAttributes3['groove'] == FileAttributes2['groove'] or FileAttributes3['groove'] == FileAttributes1['groove']:
		FileAttributes3 = parseMidi(midi)

	generator1 = ChordProg(FileAttributes1, "generated_files/accomp1.mma")
	generator1.generateMMAFormat()

	generator2 = ChordProg(FileAttributes2, "generated_files/accomp2.mma")
	generator2.generateMMAFormat()

	generator3 = ChordProg(FileAttributes3, "generated_files/accomp3.mma")
	generator3.generateMMAFormat()

	genMidi("generated_files/accomp1.mma")
	genMidi("generated_files/accomp2.mma")
	genMidi("generated_files/accomp3.mma")

	accomp1 = mido.MidiFile("generated_files/accomp1.mid")
	accomp2 = mido.MidiFile("generated_files/accomp2.mid")
	accomp3 = mido.MidiFile("generated_files/accomp3.mid")

	return (accomp1, accomp2, accomp3)

def genMidi(path_to_fakebook):
	os.system('../external/mma/mma.py -g && python ../external/mma/mma.py ' + path_to_fakebook)

if __name__ == '__main__':
	midi_file = mido.MidiFile('../../app/Design/test.mid')
	genAccompaniment(midi_file)
