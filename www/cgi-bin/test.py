#!/usr/bin/env python3
# Importing the 'cgi' module
import cgi
import sys

print("Content-type: text/html\r\n\r\n")
print("<html><body>")
print("<h1> Hello Program! </h1>")
# Using the inbuilt methods

form = cgi.FieldStorage()
for key in form.keys():
    sys.stderr.write("key: " + key + "; value: " + form.getvalue(key) + "\n")
if form.getvalue("name"):
    name = form.getvalue("name")
    print("<h1>Hello " + name + "!</h1><br />")

    val = form.getvalue("happy")
    print("<p> Value of happy is " + val + "</p>")

    val = form.getvalue("sad")
    print("<p> Value of sad is " + val + "</p>")


# Using HTML input and forms method
print("<form method='post' action='test.py' enctype='multipart/form-data'>")
print("<p>Name: <input type='text' name='name' /></p>")
print("<input type='checkbox' name='happy' /> Happy")
print("<input type='checkbox' name='sad' /> Sad")
print("<input type='submit' value='Submit' />")
print("</form")
print("</body></html>")
exit(0)
