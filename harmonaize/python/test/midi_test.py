from mido import MidiFile

def midi_test(path_to_midi):
	mid = MidiFile(path_to_midi)


if __name__ == '__main__':
	midi_test(*sys.argv[1:])


