# To use this script, run: python convert-image.py myFile.hex
# The extension .hex is suggested but will not change how program works
# The output file is given by the input file, but prepended with ascii.

import sys

print(sys.argv[1])

def writebytes(bytePair):
    if(bytePair.strip() == ""):
        return
    upper = bytePair[2:4]
    lower = bytePair[4:]

    tempFile.write(int(lower,16).to_bytes(1,'big'))
    tempFile.write(int(upper,16).to_bytes(1,'big'))


with open("ascii."+sys.argv[1], "wb") as tempFile:
    with open(sys.argv[1]) as file:
        for line in file:
            for bytePair in line.split(" "):
                writebytes(bytePair)
