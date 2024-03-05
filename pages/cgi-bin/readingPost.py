import sys
import os

data = ""
while True:
    char = sys.stdin.read(1)
    if char == "\0":
        break
    data += char

pos = data.find("name=") + 5
pos2 = data.find("&content=")
temp = data[pos:pos2]
f = open(temp, "w")
pos2 = pos2 + 9
temp = data[pos2:]
f.write(temp)
f.close()
