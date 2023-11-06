#!/usr/bin/env python3
# Importing the 'cgi' module
import cgi
import os

print("<html><body>")
# Using the inbuilt methods
# os.write(2, b"Hello World!\n")
# os.write(2, os.read(0, 10240))
# os.write(2, b"\n")

print("<a href=../cgi.html> Back to Home </a>")

form = cgi.FieldStorage()

if form.getvalue("num1") and form.getvalue("num2"):
	num1 = form.getvalue("num1")
	num2 = form.getvalue("num2")
	print("<h1>Sum of " + num1 + " and " + num2 + " is " + str(int(num1) + int(num2)) + "</h1><br />")

print("</body></html>")
exit(0)