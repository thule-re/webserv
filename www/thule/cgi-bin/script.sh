#!/bin/bash

echo "<html lang='en'><head><meta charset='UTF-8'>"
echo "<head><title>webserv CGI</title>"
echo "<link rel='stylesheet' type='text/css' href='../default.css'></head>"
echo "<header><h1>Welcome to our webserv website</h1></header>"
echo "<nav><ul><li><a href='/'>Home</a></li><li><a href='/upload.html'>Upload</a></li>"
echo "<li><a href='nonexistent.html'>Try Me!</a></li><li><a href='/cgi.html'>CGI</a></li></ul></nav>"
echo "<main><h1> CGI example 2 - shell script </h1>"
echo "Script ran successfully."
echo "</main><footer><p>&copy; 2023 The Real webserv. All rights reserved.</p></footer>"
echo "</body></html>"