/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrautne <mtrautne@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 09:40:26 by mtrautne          #+#    #+#             */
/*   Updated: 2023/08/24 09:40:26 by mtrautne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser/Config.hpp"

Config::Config() {}

Config::Config(std::string &rawConfig) {
	parseConfig(rawConfig);
}

Config::Config(const Config &other) {
	this->_configMap = other.getMap();
}

// destructor
Config::~Config() {}

// operator overload
Config &Config::operator=(const Config &other) {
	if (this == &other)
		return (*this);
	this->_configMap = other._configMap;
	return (*this);
}

std::string	getConfigValStr(const int& key) {
	switch (key) {
		case SERVERNAME:
			return "serverName";
		case ALLOWEDHTML:
			return "allowedHtml";
		case ALLOWEDMETHODS:
			return "allowedMethods";
		case ROOT:
			return "root";
		case INDEXFILE:
			return "indexFile";
		case ERRORDIR:
			return "errorDirectory";
		case CGIDIR:
			return "cgiDirectory";
		case UPLOADDIR:
			return "uploadDirectory";
		case PORT:
			return "port";
		default:
			return "unspecificConfigValue";
	}
}

// exceptions
Config::InvalidSyntaxException::InvalidSyntaxException(int key) : _key(key) {}

const char *Config::InvalidSyntaxException::what() const _NOEXCEPT {
	std::string	ret = "Invalid Config File: " + getConfigValStr(_key);
	const char *ret_c_str = ret.c_str();
	return (ret_c_str);
}

const char *Config::NotADirectoryException::what() const _NOEXCEPT {
	return ("Invalid directory in config file.");
}

const char *Config::NoValidMethodException::what() const _NOEXCEPT {
	return ("No valid Method found for server block in config file.");
}

const char *Config::InvalidHtmlException::what() const _NOEXCEPT {
	return ("Invalid Html standard.");
}

// member functions
std::map<std::string, std::string>	Config::getMap() const {
	return (this->_configMap);
}

void	Config::populateConfig(const std::string &configBlock) {
	setValue(SERVERNAME, configBlock);
	setValue(ALLOWEDHTML, configBlock);
	setValue(ALLOWEDMETHODS, configBlock);
	setValue(ROOT, configBlock);
	setValue(INDEXFILE, configBlock);
	setValue(ERRORDIR, configBlock);
	setValue(CGIDIR, configBlock);
	setValue(UPLOADDIR, configBlock);
	setValue(PORT, configBlock);
}

void	Config::setValue(const int key, const std::string &configBlock) {
	size_t	valStart;
	size_t	valEnd;

	std::string	keyStr = getConfigValStr(key);
	if (configBlock.find(keyStr) == std::string::npos)
		throw InvalidSyntaxException(key);
	else
		valStart = configBlock.find(keyStr);
	valStart = configBlock.find(':', valStart) + 2;
	valEnd = valStart;
	while (valEnd < configBlock.length() && configBlock[valEnd + 1] != ';'
			&& configBlock[valEnd + 1] != '\n')
		valEnd++;
	if (configBlock[valEnd] == '\n')
		throw InvalidSyntaxException(key);
	_configMap[keyStr] = configBlock.substr(valStart, valEnd - valStart + 1);
}

void	Config::validateDir(std::string const &directory) {
	std::string	full_path;

	if (directory.find("root") == 0)
		full_path = _configMap["root"];
	else
		full_path = _configMap["root"] + _configMap[directory];
	std::ifstream file(full_path);
	if (!file.good()) {
		std::cout <<  full_path << ": " << std::endl;
		throw NotADirectoryException();
	}
}

void	Config::validateConfigDirs() {
	try {
		validateDir("root");
		validateDir("indexFile");
		validateDir("errorDirectory");
		validateDir("cgiDirectory");
		validateDir("uploadDirectory");
	}
	catch (NotADirectoryException &e) {
	std::cout << e.what() << std::endl;
	}
}

void	Config::validateMethods() {
	if (_configMap["allowedMethods"].find("GET") == std::string::npos
		&& _configMap["allowedMethods"].find("POST") == std::string::npos
		&& _configMap["allowedMethods"].find("DELETE") == std::string::npos)
		throw NoValidMethodException();
}

void	Config::validateHtml() {
	if (_configMap["allowedHtml"] != "1.1")
		throw InvalidHtmlException();
}

void	Config::parseConfig(const std::string& configBlock) {
	std::vector<std::string> configAttributes;

	try {
		populateConfig(configBlock);
		validateConfigDirs();
		validateMethods();
		validateHtml();
	}
	catch (InvalidSyntaxException &e) {
		std::cout << e.what() << std::endl;
	}
	catch (NotADirectoryException &e) {
		std::cout << e.what() << std::endl;
	}
	catch (NoValidMethodException &e) {
		std::cout << e.what() << std::endl;
	}
	catch (InvalidHtmlException &e) {
		std::cout << e.what() << std::endl;
	}
}
