/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestGenerator.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 19:44:11 by treeps            #+#    #+#             */
/*   Updated: 2023/08/21 19:44:11 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request/RequestGenerator.hpp"

// constructors
RequestGenerator::RequestGenerator() {}

RequestGenerator::RequestGenerator(const RequestGenerator &other) {
	*this = other;
}

// destructor
RequestGenerator::~RequestGenerator() {}

// operator overload
RequestGenerator &RequestGenerator::operator=(const RequestGenerator &other) {
	if (this == &other)
		return (*this);
	return (*this);
}

ARequest *RequestGenerator::createRequest(const ClientSocket& clientSocket, const std::string &request) {
	if (request.find("GET") == 0)
		return (new GETRequest(clientSocket, request));
	else if (request.find("POST")  == 0)
		return (new POSTRequest(clientSocket, request));
	else if (request.find("DELETE") == 0)
		return (new DELETERequest(clientSocket, request));
	else
		return (new InvalidRequest(clientSocket, request));
}
