#!/usr/bin/env python3
import os

print("<html><body>")

print("<h1>" + os.getcwd() + "</h1>\n")
for folder in os.listdir():
	print("<p>" + folder + "</p>\n")

print("</body></html>")
exit(0)
