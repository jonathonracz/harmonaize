# Python Chord Progression Generation
# Adam Heins

class ChordProg():
	def __init__(self, tonic, filename, tempo, groove):
		self.keyMap = {'C': 0, 'D': 1, 'E': 2, 'F': 3, 'G': 4, 'A': 5, 'B': 6}
		self.keys = ['C', 'D', 'E', 'F', 'G', 'A', 'B']
		self.tonic = tonic
		self.filename = filename
		self.counter = 1
		self.tempo = tempo
		self.groove = groove


	def getChordFromTonic(self, chordNum, minor = False):
		newChordNum = (self.keyMap[self.tonic] + chordNum  - 1) % 7
		newChord = self.keys[newChordNum] 
		if minor:
			newChord = newChord + 'm'
		return newChord

	def basicChordProg1(self):
		chordList = []
		chordList.append(self.tonic)
		chordList.append(self.getChordFromTonic(4))
		chordList.append(self.getChordFromTonic(6, True))
		chordList.append(self.getChordFromTonic(5))
		return chordList

	def basicChordProg2(self):
		chordList = []
		chordList.append(self.tonic)
		chordList.append(self.getChordFromTonic(5))
		chordList.append(self.getChordFromTonic(6, True))
		chordList.append(self.getChordFromTonic(3, True))
		chordList.append(self.getChordFromTonic(4))
		return chordList

	def writeMMAHeader(self, file):
		file.write("// Harmonaize Accompaniment\n\n")
		file.write("Tempo " + str(self.tempo) + "\n")
		file.write("Groove " + self.groove + "\n")

	def generateMMAFormat(self):
		file = open(self.filename, 'w')
		self.writeMMAHeader(file)

		chordList = self.basicChordProg2()
		for chord in chordList:
			file.write(str(self.counter) + " " + chord + "\n")
			self.counter = self.counter + 1


		file.close()
		

if __name__ == "__main__":
	generator = ChordProg("C", "test/test.mma", 120, "Rhumba")
	generator.generateMMAFormat()
