# Python Chord Progression Generation
# Adam Heins

import math
import parse_midi as midiUtil
import mido


# class Chord():
#     chord_recipes = {'M': ['R', 'M3', 'P5'],
#                      'm': ['R', 'm3', 'P5'],
#                      'dim': ['R', 'm3', 'd5'],
#                      'aug': ['R', 'M3', 'A5'],
#                      'open5': ['R', 'P5', 'P8'],
#                      'dim7': ['R', 'm3', 'd5', 'd7'],
#                      'maj7': ['R', 'M3', 'P5', 'M7'],
#                      'aug7': ['R', 'M3', 'A5', 'm7'],
#                      'sus2': ['R', 'P5', 'P8', 'M2'],
#                      'sus4': ['R', 'P5', 'P8', 'P4']}

#     def __init__(self, root, chord_type='M'):
#         self.notes = []

#         try:
#             self.notes.append(root)
#         except:
#             raise Exception('Invalid root note supplied.')

#         if chord_type in self.chord_recipes.keys():
#             self.chord_type = chord_type
#         else:
#             raise Exception('Invalid chord type supplied! current valid types: {} '.format(self.chord_recipes.keys()))

#         self.build_chord()

#     def build_chord(self):
#         self.add_intervals(self.chord_recipes[self.chord_type][1:])

#     def add_intervals(self, intervals):
#         for i in intervals:
#             self.notes.append(self.notes[0]+Interval(i))

#     def __repr__(self):
#         return "Chord(Note({!r}), {!r})".format(str(self.notes[0]), self.chord_type)

#     def __str__(self):
#         return "{}{}".format(str(self.notes[0]),self.chord_type)

#     def __eq__(self, other):
#         if len(self.notes) != len(other.notes):
#             return False
#         else:
#             return all(self.notes[i] == other.notes[i] for i in range(len(self.notes)))


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
		self.filename = filename
		self.counter = 1
		self.tempo = FileAttributes['tempo']
		self.groove = FileAttributes['groove']

		self.num_reps = 4
		self.beat_map = FileAttributes['beat_map']
		self.time_signature = FileAttributes['time_signature']

	def genMmaFileWithExactChords(self):
		beat_map = self.condenseBeatMap(self.beat_map)

		measures = []
		measure = []
		beat_num = 1

		for beat, note_list in beat_map.items():
			if beat_num in beat_map:
				measure.append(self.getNextChord(measure, note_list))
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

		return new_map

	def getNextChord(self, chords, notes):
		if len(chords) % 2 == 1:
			return '/'
		else:
			return notes[0]

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


	def isContainedinChord(self, note, intervalFromTonic, quality):
		root = self.intervalJump(intervalFromTonic)
		return note in self.genChord(root, quality)

	def genChord(self, root, quality):
		return [self.intervalJump(interval, root) for interval in self.chordRecipes[quality]]


	def advancedChordProg(self):
		measure_num = 0
		while(True):
			if measure_num > len(self.beat_map) - 1:
				print("Done")
				print(chordList)
				return chordList

			result = self.oneMeasureAnalysis(measure_num)
		
			if result[1] == "M6":
				chordList.append(self.makeMinor(self.intervalJump("M6")))
			else:
				chordList.append(self.intervalJump(result[1]))
			measure_num = measure_num + 1



	def oneMeasureAnalysis(self, measure_num):
		INTEREST_CHORDS = [
							Chord("P1"), 
							Chord("M3"),
							Chord("P4"), 
							Chord("P5"), 
							Chord("M6", "m")]
		chord_counts = {}

		# Generate Note List
		notes = []
		note_list = self.beat_map[measure_num]
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
		print(self.filename)
		file = open(self.filename, 'w')
		self.writeMMAHeader(file)

		chordList = self.advancedChordProg()
		for chord in chordList:
			file.write(str(self.counter) + " " + chord + "\n")
			self.counter = self.counter + 1


		file.close()
		

if __name__ == "__main__":
	midi = mido.MidiFile("test/MIDI_sample.mid")
	attributes = midiUtil.parseMidi(midi)
	generator = ChordProg(attributes, "generated_files/accomp.mma")
	generator.advancedChordProg()
	# generator.generateMMAFormat()
