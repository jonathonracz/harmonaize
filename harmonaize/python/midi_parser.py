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
		self.messages = midi.tracks[0]

		time_sig_msg = getMessageWithType(self.messages, 'time_signature')
		self.time_signature = (time_sig_msg.numerator, time_sig_msg.denominator)
		self.tempo = mido.tempo2bpm(getMessageWithType(self.messages, 'set_tempo').tempo)
		self.tonic = getMessageWithType(self.messages, 'key_signature').key

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

		for message in self.messages:
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

		return (note_counts, beat_map)

def getMessageWithType(messages, msg_type):
	return next(filter(lambda msg: msg.type == msg_type, messages))





