"""python code that gets called"""
from io import BytesIO
from mido import MidiFile
from generate_midi import genAccompaniment

def openFile(p):
    b = BytesIO(p)
    midifile = MidiFile()
    midifile._load(b)
    genAccompaniment(midifile)
    f = BytesIO()
    midifile.save(file=f)
    return f.getvalue()
