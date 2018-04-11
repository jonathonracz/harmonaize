# Python class to parse MIDI messages
# Marc Lashner

import mido
import random
import math

from groove_selector import GrooveSelector
from grooves import GROOVES
from midi_notes import MODDED_NOTES

class MidiParser():
	def __init__(self, midi):
		self.meta_messages = midi.tracks[0]
		self.note_messages = midi.tracks[1]

		time_sig_msg = getMessagesWithType(self.meta_messages, 'time_signature')[0]
		self.time_signature = (time_sig_msg.numerator, time_sig_msg.denominator)
		self.tempo = mido.tempo2bpm(getMessagesWithType(self.meta_messages, 'set_tempo')[0].tempo)
		self.tonic = getMessagesWithType(self.meta_messages, 'key_signature')[0].key

		self.note_counts = makeNoteCounts(self.note_messages)
		self.measure_map = makeMeasureMap(self.tempo, self.time_signature, self.note_messages)
		print(self.measure_map)

	def getTempo(self):
		return self.tempo

	def getTimeSignature(self):
		return self.time_signature

	def getTonic(self):
		return self.tonic

	def getNoteCounts(self):
		return self.note_counts

	def getMeasureMap(self):
		return self.measure_map

def makeNoteCounts(messages):
	note_counts = {}

	for message in messages:
		if message.type == 'note_on':
			note = MODDED_NOTES[message.note % 12]
			if note in note_counts:
				note_counts[note] += 1
			else:
				note_counts[note] = 1

	return note_counts

def makeMeasureMap(tempo, time_signature, messages):
	measure_map = []
	current_beat = 0

	for message in messages:

		beat_value = message.time / 500
		current_beat += beat_value

		if message.type == 'note_on':
			current_measure = int(math.floor(current_beat / time_signature[0]))

			while current_measure >= len(measure_map):
				measure_map.append([])

			note = MODDED_NOTES[message.note % 12]
			measure_map[current_measure].append(note)

	return measure_map

def getMessagesWithType(messages, msg_type):
	msg_list = [msg for msg in messages if msg.type == msg_type]

	if len(msg_list) == 0:
		raise Exception('No matching messages')
	else:
		return msg_list





