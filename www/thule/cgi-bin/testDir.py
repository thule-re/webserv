#!/usr/bin/env python3
import os

print("<html><body>")

print("<h1>" + os.getcwd() + "</h1>\n")
for index, folder in enumerate(os.listdir()):
	print("<p>" + str(index) + ": " + folder + "</p>\n")

print("</body></html>")
exit(0)
