#!/usr/bin/env python3
# Importing the 'cgi' module
import cgi
import os

print("<html lang='en'><head><meta charset='UTF-8'>")
print("<head><title>webserv CGI</title>")
print("<link rel='stylesheet' type='text/css' href='../default.css'></head>")
print("<header><h1>Welcome to our webserv website</h1></header>")
print("<nav><ul><li><a href='/'>Home</a></li><li><a href='/upload.html'>Upload</a></li>")
print("<li><a href='nonexistent.html'>Try Me!</a></li><li><a href='/cgi.html'>CGI</a></li></ul></nav>")
print("<main><h1> Calculator </h1>")

# Using the inbuilt methods
# os.write(2, b"Hello World!\n")
# os.write(2, os.read(0, 10240))
# os.write(2, b"\n")

form = cgi.FieldStorage()

if form.getvalue("num1") and form.getvalue("num2"):
	num1 = form.getvalue("num1")
	num2 = form.getvalue("num2")
	print("<h2>The sum of " + num1 + " and " + num2 + " is " + str(int(num1) + int(num2)) + "</h2><br />")

print("</main><footer><p>&copy; 2023 The Real webserv. All rights reserved.</p></footer>")
print("</body></html>")
exit(0)