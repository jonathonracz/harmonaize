"""python code that gets called"""
from io import BytesIO
from mido import MidiFile

def openFile(p):
    b = BytesIO(p)
    midifile = MidiFile()
    midifile._load(b)
    return "done"
