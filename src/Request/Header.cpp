/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 10:32:43 by treeps            #+#    #+#             */
/*   Updated: 2023/08/25 10:32:43 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request/Header.hpp"

// constructors
Header::Header(): _rawHeader(""), _headerMap() {}

Header::Header(const std::string &rawHeader): _rawHeader(rawHeader), _headerMap() {
	_parseHeader();
}

Header::Header(const Header &other) {
	*this = other;
}

// destructor
Header::~Header() {}

// operator overload
Header &Header::operator=(const Header &other) {
	if (this == &other)
		return (*this);
	_rawHeader = other._rawHeader;
	_headerMap = other._headerMap;
	return (*this);
}

std::string &Header::operator[](const std::string &key) {
	return (_headerMap[key]);
}

// member functions
std::string Header::getHeader(const std::string &key) {
	return (_headerMap[key]);
}

void Header::_parseHeader() {
	std::string line;
	std::istringstream iss(_rawHeader);

	std::getline(iss, line);
	_parseFirstLine(line);
	while (std::getline(iss, line)) {
		_parseLine(line);
	}
}

void Header::_parseLine(const std::string &line) {
	std::string key;
	std::string value;
	size_t pos = line.find(':');
	if (pos != std::string::npos) {
		key = line.substr(0, pos);
		value = line.substr(pos + 1, line.find('\r') - pos - 1);
		_headerMap[key] = value;
	}
}

void Header::_parseFirstLine(std::string &line) {
	size_t pos = line.find(' ');
	if (pos != std::string::npos) {
		_headerMap["Method"] = line.substr(0, pos);
		line = line.substr(pos + 1);
		pos = line.find(' ');
		if (pos != std::string::npos) {
			_headerMap["Path"] = line.substr(0, pos);
			line = line.substr(pos + 1);
			_headerMap["HTTP-Version"] = line.substr(0,  line.find('\r'));
		}
	}
}
