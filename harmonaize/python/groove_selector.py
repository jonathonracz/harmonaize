# Python Groove Selector
# Marc Lashner

import random

from grooves import GROOVES

class GrooveSelector():
	def __init__(self, tempo, time_signature, genre):
		self.grooves = self.filter_grooves(tempo, time_signature, genre)

	def filter_grooves(self, tempo, time_signature, genre):
		if tempo < 90:
			grooves = GROOVES[('slow', time_signature)]
		elif tempo < 150:
			grooves = GROOVES[('regular', time_signature)]
		else:
			grooves = GROOVES[('fast', time_signature)]

		return self.filter_genre(grooves, genre)

	def filter_genre(self, grooves, genre):
			print (genre)
			if genre == 'random':
				return grooves
			else:
				groove_keys = [groove for groove in grooves if groove.lower().find(genre) != -1]
				return {groove_key: grooves[groove_key] for groove_key in groove_keys}


	def select_groove(self):
		if len(self.grooves) > 0:
			return random.choice(list(self.grooves.values()))[0]
		else:
			return random.choice(list(GROOVES.values()))[0]