import sys

data = ""
while True:
    char = sys.stdin.read(1)
    if char == "\0":
        break
    data += char

print(data)
