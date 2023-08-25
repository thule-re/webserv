/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AHeader.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 14:01:01 by treeps            #+#    #+#             */
/*   Updated: 2023/08/25 14:01:01 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Header/AHeader.hpp"

// constructors
AHeader::AHeader(): _rawHeader(""), _headerMap() {}

AHeader::AHeader(const std::string &rawHeader): _rawHeader(rawHeader), _headerMap() {}

AHeader::AHeader(const AHeader &other) {
	*this = other;
}

// destructor
AHeader::~AHeader() {}

// operator overload
AHeader &AHeader::operator=(const AHeader &other) {
	if (this == &other)
		return (*this);
	_rawHeader = other._rawHeader;
	_headerMap = other._headerMap;
	return (*this);
}

std::string &AHeader::operator[](const std::string &key) {
	return (_headerMap[key]);
}
