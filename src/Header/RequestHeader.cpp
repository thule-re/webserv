/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHeader.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 10:32:43 by treeps            #+#    #+#             */
/*   Updated: 2023/08/25 10:32:43 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Header/RequestHeader.hpp"

// constructors
RequestHeader::RequestHeader(): AHeader() {}

RequestHeader::RequestHeader(const std::string &rawHeader): AHeader(rawHeader) {
	_parseHeader();
}

RequestHeader::RequestHeader(const RequestHeader &other): AHeader(other) {
	*this = other;
}

// destructor
RequestHeader::~RequestHeader() {}

// operator overload
RequestHeader &RequestHeader::operator=(const RequestHeader &other) {
	if (this == &other)
		return (*this);
	_rawHeader = other._rawHeader;
	_headerMap = other._headerMap;
	return (*this);
}

// member functions

std::string RequestHeader::exportHeader() {
	std::string header;
	std::map<std::string, std::string>::iterator it;
	header = _headerMap["Method"] + " " + _headerMap["Path"] + " " + _headerMap["HTTP-Version"] + "\r\n";
	it = _headerMap.begin();
	if (it->first != "Method" && it->first != "Path" && it->first != "HTTP-Version") {
		header += it->first + ": " + it->second + "\r\n";
	}
	while (++it != _headerMap.end()) {
		if (it->first == "Method" || it->first == "Path" || it->first == "HTTP-Version") {
			continue;
		}
		header += it->first + ": " + it->second + "\r\n";
	}
	header += "\r\n";
	return (header);
}

void RequestHeader::_parseHeader() {
	std::string line;
	std::istringstream iss(_rawHeader);

	std::getline(iss, line);
	_parseFirstLine(line);
	while (std::getline(iss, line)) {
		_parseLine(line);
	}
}

void RequestHeader::_parseLine(const std::string &line) {
	std::string key;
	std::string value;
	size_t pos = line.find(':');
	if (pos != std::string::npos) {
		key = line.substr(0, pos);
		value = line.substr(pos + 1, line.find('\r') - pos - 1);
		removeLeadingWhitespaces(value);
		_headerMap[key] = value;
	}
}

void RequestHeader::_parseFirstLine(std::string &line) {
	size_t pos = line.find(' ');
	if (pos != std::string::npos) {
		_headerMap["Method"] = line.substr(0, pos);
		line = line.substr(pos + 1);
		pos = line.find(' ');
		if (pos != std::string::npos) {
			_headerMap["Path"] = line.substr(0, pos);
			line = line.substr(pos + 1);
			_headerMap["HTTP-Version"] = line.substr(0,line.find('\r'));
		}
	}
}
