# Webserv (Work in progress)

Welcome to the repository for our **Webserv** project, a part of the curriculum at 42.
This project focuses on creating a basic web server from scratch in C++. 
It's designed to help you understand the inner workings of how web servers handle incoming HTTP requests, 
parse them, and serve the appropriate content.

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Getting Started](#getting-started)
    - [Prerequisites](#prerequisites)
    - [Installation](#installation)
- [Usage](#usage)
- [Contributors](#contributors)
- [License](#license)

## Introduction

The **Webserv** project challenged us to build a web server capable of handling multiple incoming connections, 
parsing HTTP requests, and responding with the appropriate HTTP responses. 
We implemented support for various HTTP methods, handle static and dynamic content, 
and handle different HTTP status codes. 
This project was a fantastic opportunity to deepen our understanding of networking, 
HTTP protocol, and C++ programming.

## Features

- **HTTP/1.1 Compatible**: The server is compliant with the HTTP/1.1 protocol, capable of parsing incoming requests and generating appropriate responses.
- **Dynamic Content**: Support for server-side scripting, enabling dynamic content generation using Common Gateway Interface (CGI).
- **Static Content**: Efficiently serve static files (HTML, CSS, JS etc.) to clients.
- **Configurable**: The server's behavior is determined by a configuration file, allowing you to specify settings such as the listening port, server name, and more.

## Getting Started

### Prerequisites

Before you begin, ensure you have met the following requirements:

- A C++ compiler (e.g., `c++`)
- Unix-based operating system (Linux or macOS)

### Installation

1. Clone this repository:

   ```bash
   git clone https://github.com/your-username/webserv.git
   ```
2. Navigate to the project root directory:

   ```bash
   cd webserv
   ```

3. Compile the project:

   ```bash
   make
   ```
   
## Usage

1. Start the server:

   ```bash
   ./webserv [path_to_config_file]
   ```
   
2. Send HTTP requests to the server using your favorite web browser or a tool such as `curl` or `Postman`:

   ```bash
   curl -X GET http://localhost:8080/
   ```
   
3. Stop the server by pressing `Ctrl + C` in the terminal window.

## Contributors

In alphabetical order:

[**Maurice Trautner**](https://github.com/Mowriez)  
[**Thule Reeps**](https://github.com/thule-re)  
[**Tony Williams**](https://github.com/tonywilliamspiano)  

## License

This project uses the [MIT License](LICENSE).

---
This project was completed as part of the curriculum at [42 Wolfsburg](https://www.42wolfsburg.de/)

