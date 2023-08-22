/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POSTRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 16:11:41 by treeps            #+#    #+#             */
/*   Updated: 2023/08/21 16:11:41 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request/POSTRequest.hpp"

// constructors
POSTRequest::POSTRequest() {}
POSTRequest::POSTRequest(const ClientSocket& clientSocket, const std::string &request) : ARequest(clientSocket, request) {
	_method = "POST";
}
POSTRequest::POSTRequest(const POSTRequest &other): ARequest(other) {}

// destructor
POSTRequest::~POSTRequest() {}

// operator overload
POSTRequest &POSTRequest::operator=(const POSTRequest &other) {
	if (this == &other)
		return (*this);
	return (*this);
}


Response POSTRequest::handle() {
	Response response(_clientSocket);
	std::cout << "POSTRequest::handle()" << std::endl;
	response.buildErrorPage(501);
	return (response);
}
