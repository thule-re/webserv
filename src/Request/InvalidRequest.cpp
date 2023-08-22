/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InvalidRequest.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 16:57:56 by treeps            #+#    #+#             */
/*   Updated: 2023/08/21 16:57:56 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request/InvalidRequest.hpp"

// constructors
InvalidRequest::InvalidRequest() {}
InvalidRequest::InvalidRequest(const ClientSocket& clientSocket, const std::string &request) : ARequest(clientSocket, request) {
	_method = "INVALID";
	_code = BAD_REQUEST;
}
InvalidRequest::InvalidRequest(const ClientSocket& clientSocket, const std::string &request, int code) : ARequest(clientSocket, request), _code(code) {
	_method = "INVALID";
}
InvalidRequest::InvalidRequest(const InvalidRequest &other): ARequest(other) {}

// destructor
InvalidRequest::~InvalidRequest() {}

// operator overload
InvalidRequest &InvalidRequest::operator=(const InvalidRequest &other) {
	if (this == &other)
		return (*this);
	return (*this);
}


Response InvalidRequest::handle() {
	Response response(_clientSocket);
	response.buildErrorPage(_code);
	return (response);
}
