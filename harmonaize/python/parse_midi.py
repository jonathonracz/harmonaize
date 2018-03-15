import mido
import random
import math
from grooves import GROOVES
from midi_notes import MODDED_NOTES
from collections import defaultdict

def parseMidi(midi):
	messages = midi.tracks[0]

	FileAttributes = {
		'tempo': 0,
		'tonic': 'A',
		'time_signature': (6, 8),
		'groove': '60srock',
	}

	time_sig_msg = getMessageWithType(messages, 'time_signature')
	FileAttributes['time_signature'] = (time_sig_msg.numerator, time_sig_msg.denominator)
	FileAttributes['tempo'] = mido.tempo2bpm(getMessageWithType(messages, 'set_tempo').tempo)
	FileAttributes['tonic'] = getMessageWithType(messages, 'key_signature').key
	FileAttributes['groove'] = chooseBestGroove(FileAttributes['tempo'], FileAttributes['time_signature'])

	note_counts, beat_map = getMidiInfo(FileAttributes, messages)

	FileAttributes['note_counts'] = note_counts
	FileAttributes['beat_map'] = beat_map

	return FileAttributes
	
def chooseBestGroove(tempo, time_signature):
	if tempo < 90:
		grooves = GROOVES[('slow', time_signature)]
	elif tempo < 150:
		grooves = GROOVES[('regular', time_signature)]
	else:
		grooves = GROOVES[('fast', time_signature)]

	if len(grooves) > 0:
		return random.choice(grooves)
	else:
		return random.choice(GROOVES)

def getMessageWithType(messages, msg_type):
	return next(filter(lambda msg: msg.type == msg_type, messages))

def getMidiInfo(FileAttributes, messages):
	# Get note counts and beat mapping
	beat_map = {}
	note_counts = {}

	tempo = FileAttributes['tempo']
	current_beat = 1

	for message in messages:
		if message.type == 'note_on':

			beat_value = round((message.time * tempo / 1000 / 60) * 12) / 12
			current_beat += beat_value
			note = MODDED_NOTES[message.note % 12]
			if current_beat not in beat_map:
				beat_map[current_beat] = [note]
			else:
				beat_map[current_beat].append(note)

			if note in note_counts:
				note_counts[note] += 1
			else:
				note_counts[note] = 1
	# 		print(message)
	# print(note_counts)
	# print(beat_map)

	return (note_counts, beat_map)

if __name__ == '__main__':
	midi = mido.MidiFile("../../app/Design/test.mid")
	parseMidi(midi)