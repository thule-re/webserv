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
POSTRequest::POSTRequest(const ClientSocket& clientSocket) : ARequest(clientSocket) {
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

	getBoundary();
	_requestData = extractMultipartFormData();
	_fileData = stripHeaderFromRequest(_requestData);
	std::string filename = extractFileName(_requestData);
	filename = _clientSocket.getIndexFolder() + "/" + _clientSocket.getUploadFolder() + filename;
	writeDataToOutfile(_fileData, filename);
	response.setStatusCode(CREATED);
	return (response);
}

std::string POSTRequest::getBoundary()
{
	std::string boundary = "--" + _request.substr(_request.find("boundary=") + 9);
	size_t newlinePos = boundary.find('\n');

	return (boundary.substr(0, newlinePos));
}

std::string POSTRequest::extractMultipartFormData()
{
	int clientSocket = _clientSocket.getSocketFd();
	std::string body;
	char buffer[1024];
	unsigned long bytesRead;

	while (true) {
		bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
		if (bytesRead <= 0) {
			break;
		}
		body.append(buffer, bytesRead);
		size_t pos = body.find(_boundary);
		if (pos != std::string::npos) {
			std::string part = body.substr(0, pos);
			body.erase(0, pos + _boundary.length());
		}
		if (bytesRead < sizeof(buffer))
			break ;
	}
	return body;
}

std::string POSTRequest::stripHeaderFromRequest(const std::string& request)
{
	size_t contentStart = request.find("\r\n\r\n") + 4;
	size_t contentEnd = request.find_last_of(_boundary) - _boundary.length() - 4;

	return(request.substr(contentStart, contentEnd - contentStart));
}

void POSTRequest::writeDataToOutfile(const std::string& fileData, const std::string& filename)
{
	std::ofstream outfile(filename);

	if (!outfile.is_open()) {
		std::cerr << "Error opening file: " << filename << std::endl;
	}
	outfile << fileData << std::endl;
	outfile.close();
}

std::string POSTRequest::extractFileName(const std::string &request)
{
	std::string filename;
	size_t filenamePos;

	filenamePos = request.find("filename=\"", 0);
	if (filenamePos != std::string::npos) {
		filenamePos += 10;
		size_t filenameEndPos = request.find('\"', filenamePos);
		if (filenameEndPos != std::string::npos) {
			filename = request.substr(filenamePos, filenameEndPos - filenamePos);
		}
	}
	return (filename);
}
