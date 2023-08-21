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
InvalidRequest::InvalidRequest(ClientSocket clientSocket, const std::string &request) : ARequest(clientSocket, request) {}
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
	response.setStatusCode(501);
	response.buildErrorPage(501);
	return (response);
}
