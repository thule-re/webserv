/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHeader.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 13:57:30 by treeps            #+#    #+#             */
/*   Updated: 2023/08/25 13:57:30 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Header/ResponseHeader.hpp"

// constructors
ResponseHeader::ResponseHeader(): AHeader() {
	_headerMap["HTTP-Version"] = HTTP_VERSION;
	_headerMap["HTTP-Status-Code"] = toString(OK);
	_headerMap["HTTP-Status-Message"] = getHTTPErrorMessages(OK);
	_headerMap["Content-Type"] = "text/html";
}

ResponseHeader::ResponseHeader(const std::string &rawHeader): AHeader(rawHeader) {
	_parseHeader();
}

ResponseHeader::ResponseHeader(const ResponseHeader &other): AHeader(other) {
	*this = other;
}

// destructor
ResponseHeader::~ResponseHeader() {}

// operator overload
ResponseHeader &ResponseHeader::operator=(const ResponseHeader &other) {
	if (this == &other)
		return (*this);
	_rawHeader = other._rawHeader;
	_headerMap = other._headerMap;
	return (*this);
}

// member functions

std::string ResponseHeader::exportHeader() {
	std::string header;
	std::map<std::string, std::string>::iterator it;
	header = _headerMap["HTTP-Version"] + " " + _headerMap["HTTP-Status-Code"] + " " + _headerMap["HTTP-Status-Message"] + "\r\n";
	it = _headerMap.begin();
	if (it->first != "HTTP-Version" && it->first != "HTTP-Status-Code" && it->first != "HTTP-Status-Message") {
		header += it->first + ": " + it->second + "\r\n";
	}
	while (++it != _headerMap.end()) {
		if (it->first == "HTTP-Version" || it->first == "HTTP-Status-Code" || it->first == "HTTP-Status-Message") {
			continue;
		}
		header += it->first + ": " + it->second + "\r\n";
	}
	header += "\r\n";
	return (header);
}

void ResponseHeader::_parseHeader() {
	std::string line;
	std::istringstream iss(_rawHeader);

	std::getline(iss, line);
	_parseFirstLine(line);
	while (std::getline(iss, line)) {
		_parseLine(line);
	}
}

void ResponseHeader::_parseLine(const std::string &line) {
	std::string key;
	std::string value;
	size_t pos = line.find(':');
	if (pos != std::string::npos) {
		key = line.substr(0, pos);
		value = line.substr(pos + 1, line.find('\r') - pos - 1);
		_headerMap[key] = value;
	}
}

void ResponseHeader::_parseFirstLine(std::string &line) {
	size_t pos = line.find(' ');
	if (pos != std::string::npos) {
		_headerMap["HTTP-Version"] = line.substr(0, pos);
		line = line.substr(pos + 1);
		pos = line.find(' ');
		if (pos != std::string::npos) {
			_headerMap["HTTP-Status-Code"] = line.substr(0, pos);
			line = line.substr(pos + 1);
			_headerMap["HTTP-Status-Message"] = line.substr(0,line.find('\r'));
		}
	}
}
