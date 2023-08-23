/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 16:12:06 by treeps            #+#    #+#             */
/*   Updated: 2023/08/21 16:12:06 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_RESPONSE_HPP
# define WEBSERV_RESPONSE_HPP

# include <string>
# include <sys/socket.h>
# include <fstream>
# include <exception>
# include "Response/responseCodes.hpp"
# include "Socket/ClientSocket.hpp"
# include "utils/utils.hpp"

# define DEFAULT_ERROR_PAGE \
"<!DOCTYPE html>\n\
<html lang=\"en\">\n\
<head>\n\
    <meta charset=\"UTF-8\">\n\
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n\
    <title>STATUS_CODE STATUS_MESSAGE</title>\n\
    <style>\n\
        body {\n\
            font-family: Arial, sans-serif;\n\
            text-align: center;\n\
            margin: 0;\n\
            padding: 50px;\n\
            background-color: #f4f4f4;\n\
        }\n\
        main { \n\
            padding: 20px; \n\
        } \n\
        main h1 {\n\
            font-size: 150px;\n\
            margin: 0;\n\
            color: #e74c3c;\n\
        }\n\
        p {\n\
            font-size: 24px;\n\
            margin-top: 10px;\n\
            color: #333;\n\
        }\n\
        footer { \n\
            background-color: #333; \n\
            color: #fff; \n\
            text-align: center; \n\
            padding: 10px; \n\
            position: fixed; \n\
            left: 0; \n\
            bottom: 0; \n\
            width: 100%; \n\
        } \n\
        header h1 { \n\
            background-color: #333; \n\
            font-size: 24px; \n\
            color: #fff; \n\
            text-align: center; \n\
            padding: 10px; \n\
            position: fixed; \n\
            left: 0; \n\
            top: 0; \n\
            width: 100%; \n\
        } \n\
        footer p { \n\
            font-size: 12px; \n\
            color: #fff; \n\
        } \n\
    </style>\n\
</head>\n\
<body>\n\
    <header> \n\
        <h1>Default Webserv Error Page</h1> \n\
    </header> \n\
    <main> \n\
        <h1>STATUS_CODE</h1>\n\
        <p>STATUS_MESSAGE</p>\n\
    </main> \n\
    <footer>\n\
        <p>&copy; 2023 The Real Webserv. All rights reserved.</p>\n\
    </footer>\n\
</body>\n\
</html>"

class Response {
public:
	// constructors
	Response(const ClientSocket& clientSocket);
	Response(const Response &);

	// destructor
	~Response();

	// operator overload
	Response &operator=(const Response &);

	// member functions
	void send();
	void setStatusCode(int statusCode);
	void setBody(const std::string &body);
	void buildErrorPage(int statusCode);
	void setContentType(const std::string &contentType);

private:
	// constructors
	Response();

	// member variables
	ClientSocket _clientSocket;
	std::string _header;
	std::string	_contentType;
	std::string _body;
	std::string _statusCode;
	std::string _statusMessage;

	// member functions
	void defaultErrorPage(int statusCode);
};

std::string getHTTPErrorMessages(int statusCode);
#endif
