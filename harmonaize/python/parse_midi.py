import mido

def parse_midi(path_to_midi_file='../../app/Design/test.mid'):
	mid = mido.MidiFile(path_to_midi_file)
	messages = mid.tracks[0]

	FileAttributes = {
		'tempo': 0,
		'key': 'A',
		'time_signature': (6, 8),
	}

	time_sig_msg = get_message_with_type(messages, 'time_signature')
	FileAttributes['time_signature'] = (time_sig_msg.numerator, time_sig_msg.denominator)
	FileAttributes['tempo'] = mido.tempo2bpm(get_message_with_type(messages, 'set_tempo').tempo)
	FileAttributes['key'] = get_message_with_type(messages, 'key_signature').key

	return FileAttributes
	

def get_message_with_type(messages, msg_type):
	return next(filter(lambda msg: msg.type == msg_type, messages))

if __name__ == '__main__':
	parse_midi()
