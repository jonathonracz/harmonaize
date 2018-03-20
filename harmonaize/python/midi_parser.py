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

	def getTempo(self):
		return self.tempo

	def getTimeSignature(self):
		return self.time_signature

	def getTonic(self):
		return self.tonic

	def getMidiInfo(self):
		# Get note counts and beat mapping

		beat_map = {}
		note_counts = {}

		tempo = self.tempo
		current_beat = 0

		for message in self.note_messages:
			if message.type == 'note_on':

				beat_value = round((message.time * tempo / 1000 / 60) ) 
				current_beat += beat_value
				note = MODDED_NOTES[message.note % 12]

				relative = message.note % 12 - MODDED_NOTES.index(self.tonic)

				if relative != 0 and relative != 5 and relative != 7:
					note += 'm'

				if current_beat not in beat_map:
					beat_map[current_beat] = [note]
				else:
					beat_map[current_beat].append(note)

				if note in note_counts:
					note_counts[note] += 1
				else:
					note_counts[note] = 1

		print(beat_map)
		return (note_counts, beat_map)

def getMessagesWithType(messages, msg_type):
	msg_list = [msg for msg in messages if msg.type == msg_type]

	if len(msg_list) == 0:
		raise Exception('No matching messages')
	else:
		return msg_list





