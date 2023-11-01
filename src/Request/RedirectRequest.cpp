/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirectRequest.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 17:15:17 by treeps            #+#    #+#             */
/*   Updated: 2023/09/15 17:15:17 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request/RedirectRequest.hpp"

// constructors
RedirectRequest::RedirectRequest() {}
RedirectRequest::RedirectRequest(ClientSocket* clientSocket) : ARequest(clientSocket) {}
RedirectRequest::RedirectRequest(const RedirectRequest &other): ARequest(other) {}

// destructor
RedirectRequest::~RedirectRequest() {}

// operator overload
RedirectRequest &RedirectRequest::operator=(const RedirectRequest &other) {
	if (this == &other)
		return (*this);
	return (*this);
}

// member functions
Response *RedirectRequest::handle() {
	Response *response = new Response(_clientSocket);

	std::cout << "RedirectRequest::handle()" << std::endl;
	if (_header["Method"] == "GET" || _header["Method"] == "HEAD")
	{
		response->setHeader("Location", _location.redirect);
		response->setHeader("HTTP-Status-Code", toString(MOVED_PERMANENTLY));
		response->setHeader("HTTP-Status-Message", getHTTPErrorMessages(MOVED_PERMANENTLY));
	}
	else if (_header["Method"] == "POST")
	{
		response->setHeader("Location", _location.redirect);
		response->setHeader("HTTP-Status-Code", toString(PERMANENT_REDIRECT));
		response->setHeader("HTTP-Status-Message", getHTTPErrorMessages(PERMANENT_REDIRECT));
	}
	return (response);
}
