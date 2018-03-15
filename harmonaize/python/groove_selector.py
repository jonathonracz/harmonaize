# Python Groove Selector
# Marc Lashner

import random

from grooves import GROOVES

class GrooveSelector():
	def __init__(self, tempo, time_signature):
		self.grooves = self.filter_grooves(tempo, time_signature)

	def filter_grooves(self, tempo, time_signature):
		if tempo < 90:
			grooves = GROOVES[('slow', time_signature)]
		elif tempo < 150:
			grooves = GROOVES[('regular', time_signature)]
		else:
			grooves = GROOVES[('fast', time_signature)]
		return grooves

	def select_groove(self):
		if len(self.grooves) > 0:
			return random.choice(self.grooves)
		else:
			return random.choice(GROOVES)