#!/usr/bin/env python3
# Importing the 'cgi' module
import cgi
import os

print("<html><body>")
print("<h1> Hello Program! </h1>")
# Using the inbuilt methods
# os.write(2, b"Hello World!\n")
# os.write(2, os.read(0, 10240))
# os.write(2, b"\n")

form = cgi.FieldStorage()

if form.getvalue("name"):
    name = form.getvalue("name")
    print("<h1>Hello " + name + "!</h1><br />")

if form.getvalue("happy"):
    print("<p> Yayy! I'm happy too! </p>")

if form.getvalue("sad"):
    print("<p> Oh no! Why you sad? </p>")


# Using HTML input and forms method
print("<form method='post' action='test.py' enctype='multipart/form-data'>")
print("<input type='checkbox' name='happy' value='on' /> Happy")
print("<input type='checkbox' name='sad' value='on' /> Sad")
print("<p>Name: <input type='text' name='name' /></p>")
print("<input type='submit' value='Submit' />")
print("</form")
print("</body></html>")
exit(0)
