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
check_file = os.path.exists(temp)
f = open(temp, "w")
pos2 = pos2 + 9
temp = data[pos2:]
f.write(temp)
f.close()
if check_file:
    print("<html>\n<head>\n<title>OK</title>\n</head>\n<body>\n<center><h1>200 OK</h1></center>\n<hr>\n</body>\n</html>")
else:
    print("<html>\n<head>\n<title>Created</title>\n</head>\n<body>\n<center><h1>201 Created</h1></center>\n<hr>\n</body>\n</html>")
