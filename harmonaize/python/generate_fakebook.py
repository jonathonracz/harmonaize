from generate_midi import gen_midi

def gen_fakebook(tempo, path_to_chord_prog, groove):
	path_to_fakebook = test/fakebook.txt
	fakebook = open(path_to_fakebook, 'w+')
	chord_prog = open(path_to_chord_prog, 'r')

	# write tempo to fakebook
	fakebook.write('Tempo %d\n\n' % tempo)

	# write groove to fakebook
	fakebook.write('Groove ' + groove + '\n\n')

	# write chord progression to fakebook
	chords = chord_prog.readlines()
	for chord in chords:
		fakebook.write(chord)

	chord_prog.close()
	fakebook.close()

	gen_midi(path_to_fakebook)





