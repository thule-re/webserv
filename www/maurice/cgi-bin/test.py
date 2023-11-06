#!/usr/bin/env python3
# Importing the 'cgi' module
import cgi
import os

print("<html lang='en'><head><meta charset='UTF-8'>")
print("<head><title>webserv CGI</title>")
print("<link rel='stylesheet' type='text/css' href='../default.css'></head>")
print("<header><h1>Welcome to our webserv website</h1></header>")
print("<nav><ul><li><a href='/'>Home</a></li><li><a href='upload.html'>Upload</a></li>")
print("<li><a href='nonexistent.html'>Try Me!</a></li><li><a href='cgi.html'>CGI</a></li></ul></nav>")
print("<main><h1> Hello Program! </h1>")

# Using the inbuilt methods
# os.write(2, b"Hello World!\n")
# os.write(2, os.read(0, 10240))
# os.write(2, b"\n")

form = cgi.FieldStorage()

if form.getvalue("happy") and form.getvalue("sad"):
    print("<h1> You can't be both happy and sad! Make up your mind!</h1>")
    exit(0)

if form.getvalue("name"):
    name = form.getvalue("name")
    print("<h1>Hello " + name + "!</h1><br />")

if form.getvalue("happy"):
    print("<h1> Yayy! I'm happy too! </h1>")

if form.getvalue("sad"):
    print("<h1> Oh no! Why you sad? </h1>")


# Using HTML input and forms method
print("<form method='post' action='test.py' enctype='multipart/form-data'>")
print("<input type='checkbox' name='happy' value='on' /> Happy")
print("<input type='checkbox' name='sad' value='on' /> Sad")
print("<p>Name: <input type='text' name='name' /></p>")
print("<input type='submit' value='Submit' />")
print("</form")

print("</main><footer><p>&copy; 2023 The Real webserv. All rights reserved.</p></footer>")
print("</body></html>")
exit(0)
