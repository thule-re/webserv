/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrautne <mtrautne@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 09:40:26 by mtrautne          #+#    #+#             */
/*   Updated: 2023/09/15 17:08:34 by mtrautne         ###   ########.fr       */
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
			return "servername";
		case HTML:
			return "html";
		case METHODS:
			return "methods";
		case ROOT:
			return "root";
		case PORT:
			return "port";
		default:
			return "unspecificConfigValue";
	}
}

// exceptions
Config::ValueMissingException::ValueMissingException(const int &missingKey)
	: _key(missingKey) {}

const char *Config::ValueMissingException::what() const throw() {
	std::string	retString = "No value found for: " + getConfigValStr(_key);
	const char	*retStringC = retString.c_str();
	return (retStringC);
}

const char *Config::MissingSemicolonException::what() const _NOEXCEPT {
	return ("Error: Missing semicolon in config.");
}

const char *Config::NotADirectoryException::what() const _NOEXCEPT {
	return ("Error: Invalid directory in config.");
}

const char *Config::NoValidMethodException::what() const _NOEXCEPT {
	return ("Error: No valid method found for server block in config.");
}

const char *Config::InvalidHtmlException::what() const _NOEXCEPT {
	return ("Error: Invalid html standard in config.");
}

const char *Config::InvalidPortException::what() const _NOEXCEPT {
	return ("Error: Invalid port in config file.");
}

const char *Config::EmptyValueException::what() const _NOEXCEPT {
	return ("Error: Empty value in config file.");
}

// member functions
std::map<std::string, std::string>	Config::getMap() const {
	return (this->_configMap);
}

void	Config::populateConfig(const std::string &configBlock) {
	setGlobalValues(configBlock);
	setLocations(configBlock);
}

void 	Config::setGlobalValues(const std:: string &configBlock) {
	setValue(SERVERNAME, configBlock);
	setValue(PORT, configBlock);
	setValue(METHODS, configBlock);
	setValue(HTML, configBlock);
	setValue(ROOT, configBlock);
}

void	Config::setValue(const int key, const std::string &configBlock) {
	size_t		valStart;
	size_t		valEnd;
	std::string	keyStr;

	if (configBlock.find(getConfigValStr(key)) == std::string::npos)
		throw ValueMissingException(key);
	else
		valStart = configBlock.find(keyStr);
	valStart = configBlock.find(':', valStart) + 2;
	valEnd = valStart;
	while (valEnd < configBlock.length() && configBlock[valEnd + 1] != ';'
			&& configBlock[valEnd + 1] != '\n')
		valEnd++;
	if (configBlock[valEnd] == '\n')
		throw MissingSemicolonException();
	_configMap[keyStr] = configBlock.substr(valStart,
						(valEnd - valStart + 1));
}

void	Config::setLocations(const std::string &configBlock) {
	std::vector<std::string>	locationBlocks;
	splitLocationBlocks(locationBlocks, configBlock);
}

void	Config::splitLocationBlocks(std::vector<std::string> &locBlocks,
									const std::string &configBlock) {
	size_t	blockStart = 0;
	size_t	numLocBlocks = 0;
	size_t	blockEnd = 0;

	while (blockStart < configBlock.length() && blockEnd < configBlock.length()) {
		blockStart = configBlock.find("location", blockEnd);
		blockEnd = configBlock.find('}', blockStart);
		locBlocks.push_back(configBlock.substr(blockStart,
							(blockEnd - blockStart)));
		numLocBlocks++;
	}
	std::stringstream ss;
	ss << numLocBlocks;
	_configMap["numLocBlocks"] = ss.str();
}

void 	Config::validateNoEmptyEntry() {
	if (_configMap["html"] == ";" || _configMap["port"] == ";"
		|| _configMap["root"] == ";")
		throw EmptyValueException();
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
	validateDir("root");
//	validateDir("indexFile");
//	validateDir("errorDirectory");
//	validateDir("cgiDirectory");
//	validateDir("uploadDirectory");
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

void	Config::validatePort() {
	for (size_t i = 0; i < _configMap["port"].length(); i++) {
		if (!std::isdigit(_configMap["port"][i]))
			throw InvalidPortException();
	}
	int	portInt = stoi(_configMap["port"]); // todo: change to c++98 conforming fct
	if (portInt < 0 || portInt > 65535)
		throw InvalidPortException();
}

// catch exceptions in main to correctly close program with bad config
void	Config::parseConfig(const std::string& configBlock) {
	std::vector<std::string> configAttributes;
	populateConfig(configBlock);
	validateNoEmptyEntry();
	validateConfigDirs();
	validateMethods();
	validateHtml();
	validatePort();
}
