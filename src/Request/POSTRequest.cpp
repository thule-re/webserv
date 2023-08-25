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
POSTRequest::POSTRequest(const ClientSocket& clientSocket) : ARequest(clientSocket) {}
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

	std::cout << "_rawRequest: " << _rawRequest << std::endl;
	_getBoundary();
	_getFileData();
	_getFilename();
	_checkFilename();
	_writeDataToOutfile();
	std::ifstream file(_header["Path"].c_str());
	if (!file.is_open()) {
		throw ARequest::ARequestException(NOT_FOUND);
	}
	response.setStatusCode(CREATED);
	response.setContentType(getContentType(_header["Path"]));
	response.setBody(readFile(file));
	return (response);
}

void POSTRequest::_checkFilename()
{
	if (_filename.empty()) {
		throw ARequest::ARequestException(BAD_REQUEST);
	} else if (_filename.find("..") != std::string::npos) {
		throw ARequest::ARequestException(FORBIDDEN);
	}
	_filename = _clientSocket.getRootFolder() + "/" + _clientSocket.getUploadFolder() + "/" + _filename;
}

void POSTRequest::_getBoundary()
{
	_boundary = "--" + _rawRequest.substr(_rawRequest.find("boundary=") + 9);
	size_t newlinePos = _boundary.find('\n');

	_boundary = _boundary.substr(0, newlinePos);
}

void POSTRequest::_getFileData()
{
	size_t contentStart = _rawRequest.find("\r\n\r\n") + 4;
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
