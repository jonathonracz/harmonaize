# Python Groove Selector
# Marc Lashner

import random

from grooves import GROOVES

class GrooveSelector():
	def __init__(self, tempo, time_signature, genre):
		self.grooves = self.filter_grooves(tempo, time_signature, genre)

	def filter_grooves(self, tempo, time_signature, genre):
		grooves = {}

		if tempo < 90:
			speed = 'slow'
		elif tempo < 150:
			speed = 'regular'
		else:
			speed = 'fast'
		
		if (speed, time_signature) in GROOVES:
			grooves = GROOVES[(speed, time_signature)]
			grooves = self.filter_genre(grooves, genre)

		if len(grooves) == 0:
			all_grooves = GROOVES.values()
			all_grooves_dict = {}
			for groove_dict in all_grooves:
				all_grooves_dict.update(groove_dict)

			grooves = self.filter_genre(all_grooves_dict, genre)

		return grooves

	def filter_genre(self, grooves, genre):
			print (genre)
			if genre == 'random':
				return grooves
			else:
				groove_keys = [groove for groove in grooves if groove.lower().find(genre) != -1]
				return {groove_key: grooves[groove_key] for groove_key in groove_keys}


	def select_groove(self):
		return random.choice(list(self.grooves.values()))[0]