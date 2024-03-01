import os

def main():
	path_info = os.environ['PATH_INFO']

	try:
		path_info = path_info.replace("%40", "@")
		path_info = path_info.replace("+", " ")
		path_info = path_info[1:]
		path_info = path_info.split("&")
		for i in range(len(path_info)):
			path_info[i] = path_info[i].split("=")


		print(f"<h1>Hello there {path_info[0][1]}, welcome</h1>")

		for i in path_info:
			print("<p>")
			for j in i:
				print(j)
			print("</p>")
	except:
		print("<h1>Hello there anonymous user</h1>")

if __name__ == "__main__":
	main()