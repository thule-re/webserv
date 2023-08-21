/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 11:29:03 by treeps            #+#    #+#             */
/*   Updated: 2023/08/17 11:29:03 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_SERVER_HPP
# define WEBSERV_SERVER_HPP

# include <iostream>
# include <string>
# include <cstring>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <fstream>
# include <cstdio>

# define BUFFER_SIZE 1024
# define DEFAULT_404 \
"<!DOCTYPE html>\n\
<html lang=\"en\">\n\
<head>\n\
    <meta charset=\"UTF-8\">\n\
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n\
    <title>404 Not Found</title>\n\
    <style>\n\
        body {\n\
            font-family: Arial, sans-serif;\n\
            text-align: center;\n\
            margin: 0;\n\
            padding: 50px;\n\
            background-color: #f4f4f4;\n\
        }\n\
        h1 {\n\
            font-size: 150px;\n\
            margin: 0;\n\
            color: #e74c3c;\n\
        }\n\
        p {\n\
            font-size: 24px;\n\
            margin-top: 10px;\n\
            color: #333;\n\
        }\n\
    </style>\n\
</head>\n\
<body>\n\
    <h1>404</h1>\n\
    <p>Page Not Found</p>\n\
</body>\n\
</html>"


class Server {
public:
	// constructors
	Server();
	Server(int port, const std::string& index, const std::string& error, const std::string& folder);
	Server(const Server &);

	// destructor
	~Server();

	// operator overload
	Server &operator=(const Server &);

	// member functions
	void	init();
	void	loop();

private:
	void	handleRequest(int clientSocket);
	void	handleGETRequest(int clientSocket, const std::string& request);
	void	handlePOSTRequest(int clientSocket, const std::string& request);
	void	handleDELETERequest(int clientSocket, const std::string& request);
	void	handleInvalidRequest(int clientSocket);

	std::string extractPath(const std::string& request, int start);
	std::string getContentType(const std::string& path);
	std::string getErrorPage();

	int			_port;
	int			_serverSocket;
	std::string	_indexPath;
	std::string	_errorPath;
	std::string	_indexFolder;
};

#endif
