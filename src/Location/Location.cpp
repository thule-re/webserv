/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrautne <mtrautne@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 13:49:17 by mtrautne          #+#    #+#             */
/*   Updated: 2023/10/29 14:00:06 by mtrautne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrautne <mtrautne@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 15:43:00 by treeps            #+#    #+#             */
/*   Updated: 2023/09/27 22:02:18 by mtrautne         ###   ########.fr       */
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
					const std::string& tryFiles,
					const std::string& redirect,
					const std::string& allowedMethods,
					bool autoIndex):
					_path(path),
					_root(root),
					_index(index),
					_cgiExtension(cgi),
					_upload(upload),
					_redirect(redirect),
					_allowedMethods(allowedMethods),
					_tryFiles(tryFiles),
					_autoIndex(autoIndex) {}

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
	_tryFiles= other._tryFiles;
	_autoIndex = other._autoIndex;
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

void Location::setAutoIndex(bool autoIndex) {
	_autoIndex = autoIndex;
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

std::string Location::getAlias() const {
	return (_alias);
}

std::string Location::getTryFiles() const {
	return (_tryFiles);
}

bool Location::getAutoIndex() const {
	return (_autoIndex);
}

std::ostream& operator<<(std::ostream& output, const Location& object) {
	output << "path:" << object.getPath() << "| root:" << object.getRoot() << "| alias:"
		<< object.getAlias()<< "| index:" << object.getIndex() << "| cgiExtension:" << object.getCgiExtension()
		<< "| uploadDir:" << object.getUpload() << "| redirect:" << object.getRedirect()
		<< "| methods:" << object.getAllowedMethods()<< "| tryFiles:" << object.getTryFiles()
		<< "| autoIndex:" << object.getAutoIndex() << std::endl;
	return (output);
}
