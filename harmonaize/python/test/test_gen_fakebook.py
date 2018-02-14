import sys
sys.path.append('../')

from generate_fakebook import gen_fakebook

def test_gen_fakebook():
	tempo = 120
	groove = 'arpeggio44'
	path_to_chord_prog = 'test/chord_prog.txt'

	gen_fakebook(tempo, path_to_chord_prog, groove)

if __name__ == '__main__':
	test_gen_fakebook()