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
				   const std::string& tryFiles,
				   bool autoindex):
				   _path(path),
				   _root(root),
				   _index(index),
				   _cgi(cgi),
				   _upload(upload),
				   _tryFiles(tryFiles),
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
	_cgi = other._cgi;
	_upload = other._upload;
	_tryFiles= other._tryFiles;
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

void Location::setCgi(const std::string &cgi) {
	_cgi = cgi;
}

void Location::setUpload(const std::string &upload) {
	_upload = upload;
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

std::string Location::getCgi() const {
	return (_cgi);
}

std::string Location::getUpload() const {
	return (_upload);
}

std::string Location::getAlias() const {
	return (_alias);
}

std::string Location::getTryFiles() const {
	return (_tryFiles);
}

bool Location::getAutoindex() const {
	return (_autoindex);
}

std::ostream& operator<<(std::ostream& output, const Location& object) {
	output << "location: path:" << object.getPath() << "| root:" << object.getRoot() << "| alias:"
		<< object.getAlias()<< "| index:" << object.getIndex() << "| cgiDir:" << object.getCgi()
		<< "| uploadDir:" << object.getUpload() << "| tryFiles:" << object.getTryFiles()
		<< "| autoIndex:" << object.getAutoindex();
	return (output);
}
