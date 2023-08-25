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
DELETERequest::DELETERequest(const ClientSocket& clientSocket) : ARequest(clientSocket) {}
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
	std::cout << "DELETERequest::handle()" << std::endl;

	if (std::remove(_header["Path"].c_str()) != 0) {
		throw ARequest::ARequestException(NOT_FOUND);
	}
	response.setStatusCode(NO_CONTENT);
	return (response);
}
