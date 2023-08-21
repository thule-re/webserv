/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DELETERequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 16:11:53 by treeps            #+#    #+#             */
/*   Updated: 2023/08/21 16:11:53 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request/DELETERequest.hpp"

// constructors
DELETERequest::DELETERequest() {}
DELETERequest::DELETERequest(const ClientSocket& clientSocket, const std::string &request) : ARequest(clientSocket, request) {}
DELETERequest::DELETERequest(const DELETERequest &other): ARequest(other) {}

// destructor
DELETERequest::~DELETERequest() {}

// operator overload
DELETERequest &DELETERequest::operator=(const DELETERequest &other) {
	if (this == &other)
		return (*this);
	return (*this);
}

Response DELETERequest::handle() {
	Response response(_clientSocket);

	return (response);
}
