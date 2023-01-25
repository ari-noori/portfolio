#############################################################################
#   transform-song.py                                                       #
#                                                                           #
#   Aaron Cohen - 11/15/2022                                                #
#                                                                           #
#   Takes in a MusicXML file as a command line argument and creates         #
#   a .c array for the notes. Intended to be used with the macros and       #
#   typedefs defined in song.h.                                             #
#                                                                           #
#############################################################################

import sys
import os
import xml.etree.ElementTree as XML
from datetime import date

def get_accidental(alter):
    if alter == 0:
        return "n", "_natural"
    elif alter > 0:
        return "s" * alter, "_sharp" * alter
    elif alter < 0:
        return "b" * abs(alter), "_flat" * abs(alter)

def create_file(notes):
    date_format = "%b %d, %Y"
    file_header = '\n'.join([
            f"/*",
            f"* {name}.c",
            f"*",
            f"*  Created on: {date.today().strftime(date_format)}",
            f"*      Author: {os.getlogin()} via transform-song.py",
            f"*/"
            f"\n#include \"songs.h\"\n",
            f"const song_t {name} = {{\n",
    ])
    
    with open(f"{name}.c", "w") as songFile:
        songFile.write(file_header)

        for measure_number, measure in enumerate(notes):
            if measure.find(".attributes/key/"):
                set_key(measure.find(".attributes/key/fifths").text)
            songFile.write(f"\t// Measure {measure_number + 1}\n")
            for note in measure.findall(".note"): 
                if note.find(".pitch/step") is None:
                    songFile.write(f"\t{{\n\t\tREST,\n\t\trest\n\t}},\n")
                else: 
                    pitch = note.find(".pitch/step").text
                    octave = note.find(".pitch/octave").text
                    alter = int(note.find(".pitch/alter").text) if note.find(".pitch/alter") is not None else 0 
                    acc, accidental = get_accidental(alter)

                    songFile.write(f"\t{{\n\t\t{pitch}{acc}{octave},\n\t\t{pitch.lower()}{accidental}\n\t}},\n")
        else:
            songFile.write(f"\t// END OF SONG\n\t{{\n\t\tREST,\n\t\tEND_OF_SONG\n\t}}\n}};")

if len(sys.argv) != 2:
    print("User must provide a MusicXML file as a single command line argument")
    sys.exit()
    
name,extension = sys.argv[1].split('.')
def main():
    if(extension.upper() != "XML"):
        print("Invalid input. File must be of type MusicXML")

    root = XML.parse(sys.argv[1]).getroot()
    part_cnt = len(root.find('part-list'))
    part = None
    while part is None:
        match part_cnt:
            case 0:
                print("No parts to analyze")
                part = -1
            case 1:
                part = 1 
            case _:
                print(f"Please enter part number to transcribe (between 1 and {part_cnt}, inclusive).")
                response = input()
                if response.isnumeric() and 0 < int(response) <= part_cnt:
                    part = int(response)

    if part > 0:
        create_file(root.findall('part')[part - 1])
        print(f"{name}.c complete.")

main()
