import sys
import re
import os

print(sys.argv[1])

half_color = re.compile("^\s*(0x[0-9a-fA-F]{2},\s*)+$")
half_color_final = re.compile("^\s*(0x[0-9a-fA-F]{2},?\s*)+$")
color = re.compile("\s*(0x[0-9a-fA-F]{2}), 0x([0-9a-fA-F]{2})")

def merge_bytes(line: str) -> str:
    return '  '+', '.join([''.join(byte_pair.groups()) for byte_pair in color.finditer(line)])

with open("swp."+sys.argv[1], "w") as tempFile:
    with open(sys.argv[1]) as file:
        for line in file:
            if half_color.match(line):
                tempFile.write(merge_bytes(line))
                tempFile.write(',\n')
            elif half_color_final.match(line):
                tempFile.write(merge_bytes(line))
                tempFile.write('\n')
            else:
                tempFile.write(line)
