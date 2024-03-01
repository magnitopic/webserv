import sys

data = ""
while True:
    char = sys.stdin.read(1)
    if char == "\0":
        break
    data += char
pos = (data.rfind("content=") + 8)
if pos < 8:
    pos = 0

print("<html>\n<body>\n<h1>The text introduced was:</h1><p>\"" + data[pos:].lstrip() + "\"</p>\n</body>\n</html>\0")
