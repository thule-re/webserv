/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 15:43:00 by treeps            #+#    #+#             */
/*   Updated: 2023/09/08 15:43:00 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location/Location.hpp"

// constructors
Location::Location() {}

Location::Location(const std::string& path,
				   const std::string& root,
				   const std::string& index,
				   const std::string& cgi,
				   const std::string& upload,
				   const std::string& redirect,
				   const std::string& allowedMethods,
				   bool autoindex):
		_path(path),
		_root(root),
		_index(index),
		_cgiExtension(cgi),
		_upload(upload),
		_redirect(redirect),
		_allowedMethods(allowedMethods),
		_autoindex(autoindex) {}

Location::Location(const Location &other) {
	*this = other;
}

// destructor
Location::~Location() {}

// operator overload
Location &Location::operator=(const Location &other) {
	if (this == &other)
		return (*this);
	_path = other._path;
	_root = other._root;
	_index = other._index;
	_cgiExtension = other._cgiExtension;
	_upload = other._upload;
	_redirect = other._redirect;
	_allowedMethods = other._allowedMethods;
	_autoindex = other._autoindex;
	return (*this);
}

// member functions

// setter functions

void Location::setPath(const std::string &path) {
	_path = path;
}

void Location::setRoot(const std::string &root) {
	_root = root;
}

void Location::setIndex(const std::string &index) {
	_index = index;
}

void Location::setCgiExtension(const std::string &cgi) {
	_cgiExtension = cgi;
}

void Location::setUpload(const std::string &upload) {
	_upload = upload;
}

void Location::setRedirect(const std::string &redirect) {
	_redirect = redirect;
}

void Location::setAllowedMethods(const std::string &allowedMethods) {
	_allowedMethods = allowedMethods;
}

void Location::setAutoindex(bool autoindex) {
	_autoindex = autoindex;
}

// getter functions

std::string Location::getPath() const {
	return (_path);
}

std::string Location::getRoot() const {
	return (_root);
}

std::string Location::getIndex() const {
	return (_index);
}

std::string Location::getCgiExtension() const {
	return (_cgiExtension);
}

std::string Location::getUpload() const {
	return (_upload);
}

std::string Location::getRedirect() const {
	return (_redirect);
}

std::string Location::getAllowedMethods() const {
	return (_allowedMethods);
}

bool Location::getAutoindex() const {
	return (_autoindex);
}
