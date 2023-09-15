/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POSTRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tony <tony@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 16:11:41 by treeps            #+#    #+#             */
/*   Updated: 2023/08/26 13:53:48 by tony             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request/POSTRequest.hpp"

// constructors
POSTRequest::POSTRequest() {}
POSTRequest::POSTRequest(ClientSocket* clientSocket) : ARequest(clientSocket) {}
POSTRequest::POSTRequest(const POSTRequest &other): ARequest(other) {}

// destructor
POSTRequest::~POSTRequest() {}

// operator overload
POSTRequest &POSTRequest::operator=(const POSTRequest &other) {
	if (this == &other)
		return (*this);
	return (*this);
}

Response *POSTRequest::handle() {
	Response *response = new Response(_clientSocket);
	std::cerr << "POSTRequest::handle()" << std::endl;

	std::ofstream out("rawRequest.txt");
	out << _rawRequest;
	out.close();

	_getBoundary();
	if (atoi(_header["Content-Length"].c_str()) > MAX_FILE_SIZE)
		throw ARequest::ARequestException(REQUEST_ENTITY_TOO_LARGE);
	_getFileData();
	_getFilename();
	_checkFilename();
	_writeDataToOutfile();
	return (response);
}

void POSTRequest::_checkFilename()
{
	if (_filename.empty()) {
		throw ARequest::ARequestException(BAD_REQUEST);
	} else if (_filename.find("..") != std::string::npos) {
		throw ARequest::ARequestException(FORBIDDEN);
	}
	_filename = _location->getRoot() + "/" + _location->getUpload() + "/" + _filename;
}

void POSTRequest::_getBoundary()
{
	_boundary = "--" + _rawRequest.substr(_rawRequest.find("boundary=") + 9);
	size_t newlinePos = _boundary.find('\n');

	_boundary = _boundary.substr(0, newlinePos);
}

void POSTRequest::_getFileData()
{
	size_t contentStart = _rawRequest.find(CRLF CRLF);
	contentStart = _rawRequest.find(CRLF CRLF,contentStart + 5) + 4;
	size_t contentEnd = _rawRequest.find_last_of(_boundary) - _boundary.length() - 4;

	_fileData = _rawRequest.substr(contentStart, contentEnd - contentStart);
}

void POSTRequest::_writeDataToOutfile()
{
	std::ofstream outfile(_filename);

	if (!outfile.is_open()) {
		std::cerr << "Error opening file: " << _filename << std::endl;
	}
	outfile << _fileData << std::endl;
	outfile.close();
}

void POSTRequest::_getFilename()
{
	size_t filenamePos;

	filenamePos = _rawRequest.find("filename=\"", 0);
	if (filenamePos != std::string::npos) {
		filenamePos += 10;
		size_t filenameEndPos = _rawRequest.find('\"', filenamePos);
		if (filenameEndPos != std::string::npos) {
			_filename = _rawRequest.substr(filenamePos, filenameEndPos - filenamePos);
		}
	}
}
