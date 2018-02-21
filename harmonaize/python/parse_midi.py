import mido
from grooves import GROOVES

def parseMidi(midi):
	messages = midi.tracks[0]

	FileAttributes = {
		'tempo': 0,
		'key': 'A',
		'time_signature': (6, 8),
		'groove': '60srock',
	}

	time_sig_msg = getMessageWithType(messages, 'time_signature')
	FileAttributes['time_signature'] = (time_sig_msg.numerator, time_sig_msg.denominator)
	FileAttributes['tempo'] = mido.tempo2bpm(getMessageWithType(messages, 'set_tempo').tempo)
	FileAttributes['key'] = getMessageWithType(messages, 'key_signature').key
	FileAttributes['groove'] = chooseBestGroove(FileAttributes['tempo'], FileAttributes['time_signature'])

	return FileAttributes
	
def chooseBestGroove(tempo, time_signature):
	if tempo < 90:
		grooves = GROOVES[('slow', time_signature)]
	elif tempo < 150:
		grooves = GROOVES[('regular', time_signature)]
	else:
		grooves = GROOVES[('fast', time_signature)]
	return grooves[0]

def getMessageWithType(messages, msg_type):
	return next(filter(lambda msg: msg.type == msg_type, messages))

if __name__ == '__main__':
	parse_midi()