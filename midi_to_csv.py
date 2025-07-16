import csv
import os
import pretty_midi
import sys
from operator import itemgetter, attrgetter

class Note:
	def __init__(self,p,ont,offt,d,vel):
		self.pitch=p
		self.start=float(ont)
		self.end=float(offt)
		self.duration=float(d)
		self.velocity=int(vel)
		
	def __str__(self):
		return str(self.__dict__)

def process_midi(midi_file):
    print("processing " +  midi_file)
    csv_file = midi_file.replace(".mid", ".csv")
    # Load MIDI file into PrettyMIDI object
    midi_data = pretty_midi.PrettyMIDI(midi_file)
    midi_list = []
    with open(csv_file, mode='w') as output_file:
        csv_writer = csv.writer(output_file, delimiter=' ', quotechar='"', quoting=csv.QUOTE_MINIMAL, lineterminator='\n')
        csv_writer.writerow(["pitch", "start", "end", "duration", "velocity"])
        for instrument in midi_data.instruments:
            for note in instrument.notes:
                midi_list.append(Note(note.pitch,note.start,note.end,note.duration,note.velocity))
        midi_list.sort(key=attrgetter('start'))
        for note in midi_list:
            csv_writer.writerow([pretty_midi.note_number_to_name(note.pitch), "{:0>3.4f}".format(note.start), "{:0>3.4f}".format(note.end), "{:3.4f}".format(note.duration), note.velocity])



try:
    midi_dir = sys.argv[1]
except IndexError:
    raise SystemExit(f"Usage: {sys.argv[0]} <midi directory>")

if midi_dir.endswith(".mid"):
    process_midi(midi_dir)
