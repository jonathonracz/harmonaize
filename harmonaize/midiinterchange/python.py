"""python code that gets called"""
import sys
sys.path.append('../python')
from io import BytesIO
from mido import MidiFile
from generate_midi import genAccompaniment

def openFile(p):
    b = BytesIO(p)
    midifile = MidiFile()
    midifile._load(b)
    newFile = genAccompaniment(midifile)
    f = BytesIO()
    newFile.save(file=f)
    print(f.getvalue())
    return f.getvalue()
