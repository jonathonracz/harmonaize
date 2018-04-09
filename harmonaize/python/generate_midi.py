#!/usr/bin/env python 

import os, sys
sys.path.append(os.path.abspath(os.path.join('..', '..', '..', '..', '..', '..', '..', '..', 'harmonaize', 'external', 'mido')))

import mido
import tempfile

from chord_prog import ChordProg
from midi_parser import MidiParser
from groove_selector import GrooveSelector

from grooves import GROOVES

PATH_TO_TEMP_DIR = tempfile.mkdtemp()

def genAccompaniment(midi=None):
	parser = MidiParser(midi)
	selector = GrooveSelector(parser.getTempo(), parser.getTimeSignature())

	FileAttributes = {
		'tempo': parser.getTempo(),
		'tonic': parser.getTonic(),
		'time_signature': parser.getTimeSignature(),
		'note_counts': parser.getNoteCounts(),
		'measure_map': parser.getMeasureMap(),
		'groove': selector.select_groove(),
	}

	generator = ChordProg(FileAttributes, PATH_TO_TEMP_DIR + "/accomp.mma")
	generator.generateMMAFormat()

	genMidi(PATH_TO_TEMP_DIR + "/accomp.mma")
	return mido.MidiFile(PATH_TO_TEMP_DIR + "/accomp.mid")

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
	os.system('python mma/mma.py -G > ' + PATH_TO_TEMP_DIR + '/junk && python mma/mma.py ' + path_to_fakebook + ' > ' + PATH_TO_TEMP_DIR + '/junk')

if __name__ == '__main__':
	midi_file = mido.MidiFile('example.mid')
	genAccompaniment(midi_file)
