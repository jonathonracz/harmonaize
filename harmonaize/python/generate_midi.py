#!/usr/bin/env python 

import os
import sys
from parse_midi import parse_midi
from grooves import GROOVES

def genMidi(midi_file=None):
	FileAttributes = parse_midi(midi_file)

def main(path_to_fakebook):
	os.system('python3 ../mma.py ' + path_to_fakebook)

if __name__ == '__main__':
    main(*sys.argv[1:])
