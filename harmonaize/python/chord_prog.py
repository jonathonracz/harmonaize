# Python Chord Progression Generation
# Adam Heins

import math
import mido


class Chord():
	def __init__(self, number, chord_type='M'):
		self.num = number
		self.type = chord_type

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

		self.chordRecipes = {'M': ['P1', 'M3', 'P5'],
					 'm': ['P1', 'm3', 'P5'],
					 'dim': ['P1', 'm3', 'd5'],
					 'aug': ['P1', 'M3', 'A5'],
					 'open5': ['P1', 'P5', 'P8'],
					 'dim7': ['P1', 'm3', 'd5', 'd7'],
					 'maj7': ['P1', 'M3', 'P5', 'M7'],
					 'aug7': ['P1', 'M3', 'A5', 'm7'],
					 'sus2': ['P1', 'P5', 'P8', 'M2'],
					 'sus4': ['P1', 'P5', 'P8', 'P4']}                 

		self.majorScale = ['M2', 'M3', 'P4', 'P5', 'M6', 'M7', 'P8']
		self.tonic = FileAttributes['tonic']
		self.quality = 'major'
		self.filename = filename
		self.counter = 1
		self.tempo = FileAttributes['tempo']
		self.groove = FileAttributes['groove']
		self.num_reps = 4
		self.measure_map = FileAttributes['measure_map']
		self.time_signature = FileAttributes['time_signature']
		if self.tonic[-1] == "m":
			self.tonic = self.tonic[0]
			self.quality = 'minor'

	def intervalJump(self, interval, root=None):
		if root is None:
			root = self.tonic
		newNote = (self.keyMap[root] + self.semitones[interval]) % 12
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

	def basicChordProg(self):
		chordList = []
		for _ in range(4):
			chordList.append(self.tonic)
			chordList.append(self.intervalJump("P5"))
			chordList.append(self.makeMinor(self.intervalJump("M6")))
			chordList.append(self.intervalJump("P4"))
		return chordList


	def isContainedinChord(self, note, intervalFromTonic, quality):
		root = self.intervalJump(intervalFromTonic)
		return note in self.genChord(root, quality)

	def genChord(self, root, quality):
		return [self.intervalJump(interval, root) for interval in self.chordRecipes[quality]]


	def advancedChordProg(self):
		measure_num = 0
		chordList = []
		while(True):
			if measure_num > len(self.measure_map) - 1:
				print("Done")
				print(chordList)
				return chordList

			result = self.oneMeasureAnalysis(measure_num)
			
			if not result[1]:
				chordList.append(self.tonic)
			elif result[1] == "M6":
				chordList.append(self.makeMinor(self.intervalJump("M6")))
			else:
				chordList.append(self.intervalJump(result[1]))
			measure_num = measure_num + 1



	def oneMeasureAnalysis(self, measure_num):
		if self.quality == "major":
			INTEREST_CHORDS = [
							Chord("P1"), 
							Chord("M3"),
							Chord("P4"), 
							Chord("P5"), 
							Chord("M6", "m")]
		else:
			INTEREST_CHORDS = [
							Chord("P1", "m"), 
							Chord("P4", "m"), 
							Chord("P5"), 
							Chord("M6", "M")]
		
		chord_counts = {}

		# Generate Note List
		notes = []
		note_list = self.measure_map[measure_num]
		for note in note_list:
			notes.append(note)

		# Alert that we're done
		if not notes:
			return (False, False)

		# Generate Chord Counts
		for chord in INTEREST_CHORDS:
			for note in notes:
				if self.isContainedinChord(note, chord.num, chord.type):
					if chord.num not in chord_counts:
						chord_counts[chord.num] = 1
					else:
						chord_counts[chord.num] = chord_counts[chord.num] + 1

		max_count = 0
		best_chord = "P1"
		for chord_num, count in chord_counts.items():
			if count > max_count:
				max_count = count
				best_chord = chord_num
		return (True, best_chord)



	def writeMMAHeader(self, file):
		file.write("// Harmonaize Accompaniment\n\n")
		file.write("Tempo " + str(self.tempo) + "\n")
		file.write("Groove " + self.groove + "\n")

	def generateMMAFormat(self):
		file = open(self.filename, 'w')
		self.writeMMAHeader(file)

		if len(self.measure_map) == 0:
			chordList = self.basicChordProg()
		else:
			chordList = self.advancedChordProg()
			
		for chord in chordList:
			file.write(str(self.counter) + " " + chord + "\n")
			self.counter = self.counter + 1


		file.close()
		

# if __name__ == "__main__":
# 	midi = mido.MidiFile("test/MIDI_sample.mid")
# 	# attributes = midiUtil.parseMidi(midi)
# 	generator = ChordProg(attributes, "generated_files/accomp.mma")
# 	generator.advancedChordProg()
# 	# generator.generateMMAFormat()
