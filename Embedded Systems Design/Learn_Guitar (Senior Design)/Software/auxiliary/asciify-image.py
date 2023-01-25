import sys
import binascii
from PIL import Image
from itertools import chain

def asciify(pixel):
    binary_string = "{:05b}{:06b}{:05b}".format(*pixel)
    bits = int(binary_string, 2)
    hi = bits >> 8
    lo = bits & 0xFF
    print(hi,lo)
    return (chr(hi), chr(lo))

print(sys.argv[1])

# .convert goes to 24bit (8/8/8) colors
# .point operates at each color band, changing 8 bit colors to 3 bit colors
compressed_image = Image \
    .open(sys.argv[1]) \
    .convert('RGB') \
    .point(lambda pixel: pixel >> 3)

with open("./out.txt", "w") as out: 
    data = chain(*map(asciify, compressed_image.getdata()))
    out.write(''.join(list(data)))

# Save compressed image for debugging
compressed_image.point(lambda pixel: pixel << 2).save("./out.png")
