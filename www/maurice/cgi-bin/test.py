#!/usr/bin/env python3
# Importing the 'cgi' module
import cgi
import os

print("<html lang='en'><head><meta charset='UTF-8'>")
print("<title>webserv CGI</title>")
print("<style> body, h1, h2, h3, p, ul, li {margin: 0; padding: 0;}")
print("body {background-color: #444;font-family: Arial, sans-serif;}")
print("header {background-color: #64b3e3; color: #fff; text-align: center; padding: 20px;}")
print("nav ul {list-style: none;background-color: #333;text-align: center;padding: 10px;}")
print("nav ul li {display: inline;margin-right: 10px;}")
print("nav ul li a {color: #fff; text-decoration: none;}")
print("main {padding: 20px;color: #fff;}")
print("footer {background-color: #333;color: #fff; text-align: center; padding: 10px; position: fixed;")
print("left: 0; bottom: 0; width: 100%;}")
print("main p {margin-bottom: 20px; line-height: 1.6;}")
print("footer p {font-size: 12px;}")
print("nav ul li a:hover {color: #ff9900;}")
print("h1 {color: #fff;}")
print("form {color: #bbb;}")
print("checkbox {margin-left: 103px;}")
print("button {margin-left: 103px;}</style>")
print("<header><h1>Welcome to our webserv website</h1></header>")
print("<nav><ul><li><a href='/'>Home</a></li><li><a href='upload.html'>Upload</a></li>")
print("<li><a href='nonexistent.html'>Try Me!</a></li><li><a href='cgi.html'>CGI</a></li></ul></nav>")
print("<main><h1> Hello Program! </h2>")

# Using the inbuilt methods
# os.write(2, b"Hello World!\n")
# os.write(2, os.read(0, 10240))
# os.write(2, b"\n")

form = cgi.FieldStorage()

if form.getvalue("happy") and form.getvalue("sad"):
    print("<h1> You can't be both happy and sad! Make up your</h1>")
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
