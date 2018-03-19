# Python Chord Progression Generation
# Adam Heins

import math
import random

class ChordProg():
	def __init__(self, FileAttributes, filename):
		self.keyMap = {
		 	 'C': 0, 
			 'C#': 1, 'Db': 1,
			 'D': 2, 
			 'D#': 3, 'Eb': 3,
			 'E': 4, 
			 'F': 5, 
			 'F#': 6, 'Gb': 6,
			 'G': 7, 
			 'G#': 8, 'Ab': 8,
			 'A': 9, 
			 'A#': 10, 'Bb': 10,
			 'B': 11}

		self.keys = {
			 0: 'C', 
			 1: 'C#', # 1: 'Db',
			 2: 'D', 
			 3: 'D#', # 3: 'Eb',
			 4: 'E', 
			 5: 'F', 
			 6: 'F#', # 6: 'Gb',
			 7: 'G', 
			 8: 'G#', # 8: 'Ab',
			 9: 'A', 
			 10: 'A#', # 10: 'Bb',
			 11: 'B'}

		self.semitones = {'P1': 0, 'A1':1, 'd2':0, 'm2':1, 'M2':2, 'A2':3,
                              'd3':3, 'm3':3, 'M3':4, 'A3':5, 'd4':4, 'P4':5,
                              'A4':6, 'd5':6, 'P5':7, 'A5':8, 'd6':7, 'm6':8,
                              'M6':9, 'A6':10,'d7':9, 'm7':10, 'M7':11, 'A7':12,
                              'd8':11, 'P8':12}

		self.majorScale = ['M2', 'M3', 'P4', 'P5', 'M6', 'M7', 'P8']
		self.tonic = FileAttributes['tonic']
		self.filename = filename
		self.counter = 1
		self.tempo = FileAttributes['tempo']
		self.groove = FileAttributes['groove']

		self.num_reps = 4
		self.beat_map = FileAttributes['beat_map']
		self.time_signature = FileAttributes['time_signature']

	def genMmaFileWithExactChords(self):
		# beat_map = self.condenseBeatMap(self.beat_map)
		beat_map = self.beat_map

		measures = []
		measure = []
		beat_num = 1

		for beat, note_list in beat_map.items():
			if beat_num in beat_map:
				measure.append(self.getNextChord(measure, note_list))

			elif beat_num % self.time_signature[0] == 1:
				measure.append(self.tonic)

			else:
				measure.append('/')

			if beat_num % self.time_signature[0] == 0:
				measures.append(measure)
				measure = []

			beat_num += 1

		file = open(self.filename, 'w')
		self.writeMMAHeader(file)

		measure_num = 1

		for _ in range(self.num_reps):
			for measure_notes in measures:

				file.write(str(measure_num))
				for note in measure_notes:
					file.write(" " + note)
				file.write('\n')

				measure_num += 1

		file.close()

	def condenseBeatMap(self, beat_map):
		print(beat_map)
		new_map = {}
		current_beat = 1
		for beat, note_list in beat_map.items():
			if beat - current_beat >= 1:
				current_beat += math.floor(beat - current_beat)

			if current_beat in new_map:
				for note in note_list:
					new_map[current_beat].append(note)
			else:
				new_map[current_beat] = note_list

		print(new_map)
		return new_map

	def getNextChord(self, chords, notes):
		print(chords, notes)
		if notes[0] == '/' and len(chords) == 0:
			return self.tonic
		if len(chords) % 2 == 1:
			return chords[len(chords)-1]
		else:
			return notes[0]

	def intervalJump(self, interval):
		newNote = (self.keyMap[self.tonic] + self.semitones[interval]) % 12
		return self.keys[newNote]

	def makeMinor(self, chord):
		newChord = chord + 'm'
		return newChord


	def getChordFromTonic(self, chordNum, minor = False):
		oldChordNum = self.keyMap[self.tonic]

		newChordNum = (self.keyMap[self.tonic] + chordNum  - 1) % 7
		newChord = self.keys[newChordNum] 
		if minor:
			newChord = newChord + 'm'
		return newChord

	def basicChordProg1(self):
		chordList = []
		chordList.append(self.tonic)
		chordList.append(self.intervalJump("P4"))
		chordList.append(self.makeMinor(self.intervalJump("M6")))
		chordList.append(self.intervalJump("P5"))
		return chordList

	def basicChordProg2(self):
		chordList = []
		chordList.append(self.tonic)
		chordList.append(self.intervalJump("P5"))
		chordList.append(self.makeMinor(self.intervalJump("M6")))
		chordList.append(self.makeMinor(self.intervalJump("M3")))
		chordList.append(self.intervalJump("P4"))
		print(chordList)
		return chordList

	def writeMMAHeader(self, file):
		file.write("// Harmonaize Accompaniment\n\n")
		file.write("Tempo " + str(self.tempo) + "\n")
		file.write("Groove " + self.groove + "\n")

	def generateMMAFormat(self):
		print(self.filename)
		file = open(self.filename, 'w')
		self.writeMMAHeader(file)

		chordList = self.basicChordProg2()
		for chord in chordList:
			file.write(str(self.counter) + " " + chord + "\n")
			self.counter = self.counter + 1


		file.close()
		

if __name__ == "__main__":
	generator = ChordProg("C", "generated_files/accomp.mma", 120, "Rhumba")
	generator.generateMMAFormat()
