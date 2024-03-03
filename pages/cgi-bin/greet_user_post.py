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

data = data[pos:]
print("<html>\n<body>\n<h1>The input introduced was:</h1>")
length = len(data)
while pos <= length and pos > 0:
    pos = (data.find("&") + 1)
    print("<p>" + data[0 : pos - 1] + "</p>\n")
    data = data[pos:]
print("</body>\n</html>\0")
