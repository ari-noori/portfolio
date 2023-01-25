import sys
import binascii
from PIL import Image

def parse(entry):
    hi,lo = map(ord,entry)
    print(hi,lo)
    return (hi << 8) | lo


length = int(sys.argv[2])
width = int(sys.argv[3])
with open(sys.argv[1], "r") as src:
    asciidata = src.readline()
    data = map(parse, zip(asciidata[::2], asciidata[1::2]))
    image = Image.new('RGB', (length, width))
    image.putdata(list(data))
    image.point(lambda x: x << 3).save('./from_ascii.png')
    
    print(image.getpixel((0,0)))
    print(image.getpixel((0,1)))
