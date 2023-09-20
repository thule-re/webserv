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
#include "utils/utils.hpp"

Config::Config() {}

Config::Config(std::string &rawConfig) {
	parseConfig(rawConfig);
}

Config::Config(const Config &other) {
	this->_configMap = other.getMap();
	this->_locations = other.getLocations();
}

// destructor
Config::~Config() {}

// operator overload
Config &Config::operator=(const Config &other) {
	if (this == &other)
		return (*this);
	this->_configMap = other.getMap();
	this->_locations = other.getLocations();
	return (*this);
}

std::string	getValStr(const int& key) {
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
	std::string	retString = "No value found for: " + getValStr(_key);
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

const char *Config::MissingClosingBracketException::what() const _NOEXCEPT {
	return ("Error: Missing closing brackets for location in config file.");
}

// member functions
std::map<std::string, std::string>	Config::getMap() const {
	return (this->_configMap);
}

std::vector<Location>	Config::getLocations() const {
	return (this->_locations);
}

void 	Config::populateGlobalVarsMap(const std:: string &configBlock) {
	std::string globalVarsBlock;

	globalVarsBlock = extractglobalVarsBlock(configBlock);
	setConfigValue(SERVERNAME, globalVarsBlock);
	setConfigValue(PORT, globalVarsBlock);
	setConfigValue(HTML, globalVarsBlock);
	setConfigValue(ROOT, globalVarsBlock);
}

void	extractglobalVarsBlock(std::string &configBlock) {
	std::
}

void	Config::setConfigValue(const int key, const std::string &configBlock) {
	size_t		valStart;
	size_t		valEnd;
	std::string	keyStr;

	keyStr = getValStr(key);
	if (configBlock.find(keyStr) == std::string::npos)
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
	for (size_t i = 0; i < locationBlocks.size(); i++) {
		populateLocation(locationBlocks[i]);
	}
}

void	Config::populateLocation(std::string &locationBlock) {
	std::string	path = extractPath(locationBlock);
	std::string	root = extractRoot(locationBlock);
	std::string	index = extractIndex(locationBlock);
	std::string	cgi = extractCgi(locationBlock);
	std::string	upload = extractUpload(locationBlock);
	std::string	tryFiles = extractTryFiles(locationBlock);
	std::string	redirect = extractRedirect(locationBlock);
	std::string methods = extractMethods(locationBlock);
	bool	autoIndex = extractAutoIndex(locationBlock);
	Location	location(path, root, index, cgi, upload, tryFiles, redirect, methods, autoIndex);
	_locations.push_back(location);
}

std::string	Config::extractRedirect(const std::string &locationBlock) {
	if (locationBlock.find("httpRedir:") == std::string::npos)
		return ("");
	size_t start = locationBlock.find("httpRedir:") + 11;
	size_t end = locationBlock.find('{', start);
	std::string value = locationBlock.substr(start, end - start - 1);
	return (value);
}

std::string	Config::extractMethods(const std::string &locationBlock) {
	if (locationBlock.find("methods:") == std::string::npos)
		return ("");
	size_t start = locationBlock.find("methods:") + 9;
	size_t end = locationBlock.find(';', start);
	//if (locationBlock.find('\n', start) < locationBlock.find(';', start))
	//	throw MissingSemicolonException();
	std::string value = locationBlock.substr(start, end - start);
	replaceAll(value, ", ", "");
	std::cout << value << std::endl;
	return (value);
}

std::string	Config::extractPath(const std::string &locationBlock) {
	if (locationBlock.find("location") == std::string::npos)
		return ("");
	size_t start = locationBlock.find("location") + 9;
	size_t end = locationBlock.find(';', start);
	// if (locationBlock.find('\n', start) < locationBlock.find(';', start))
	// 	throw MissingSemicolonException();
	std::string value = locationBlock.substr(start, end - start - 1);
	return (value);
}

std::string	Config::extractRoot(const std::string &locationBlock) {
	if (locationBlock.find("root:") == std::string::npos)
		return ("");
	size_t start = locationBlock.find("root:") + 6;
	size_t end = locationBlock.find(';', start);
	// if (locationBlock.find('\n', start) < locationBlock.find(';', start))
	// 	throw MissingSemicolonException();
	std::string value = locationBlock.substr(start, end - start);
	return (value);
}

std::string	Config::extractIndex(const std::string &locationBlock) {
	if (locationBlock.find("index:") == std::string::npos)
		return ("");
	size_t start = locationBlock.find("index:") + 7;
	size_t end = locationBlock.find(';', start);
	// if (locationBlock.find('\n', start) < locationBlock.find(';', start))
	// 	throw MissingSemicolonException();
	std::string value = locationBlock.substr(start, end - start);
	return (value);
}

std::string	Config::extractCgi(const std::string &locationBlock) {
	if (locationBlock.find("cgiDir:") == std::string::npos)
		return ("");
	size_t start = locationBlock.find("cgiDir:") + 8;
	size_t end = locationBlock.find(';', start);
	//if (locationBlock.find('\n', start) < locationBlock.find(';', start))
	//	throw MissingSemicolonException();
	std::string value = locationBlock.substr(start, end - start);
	return (value);
}

std::string	Config::extractUpload(const std::string &locationBlock) {
	if (locationBlock.find("uploadDir:") == std::string::npos)
		return ("");
	size_t start = locationBlock.find("uploadDir:") + 11;
	size_t end = locationBlock.find(';', start);
	if (locationBlock.find('\n', start) < locationBlock.find(';', start))
		throw MissingSemicolonException();
	std::string value = locationBlock.substr(start, end - start);
	return (value);
}

std::string	Config::extractTryFiles(const std::string &locationBlock) {
	if (locationBlock.find("tryFiles:") == std::string::npos) {
		return ("");
	}
	size_t start = locationBlock.find("tryFiles:") + 10;
	size_t end = locationBlock.find(';', start);
	if (locationBlock.find('\n', start) < locationBlock.find(';', start))
		throw MissingSemicolonException();
	std::string value = locationBlock.substr(start, end - start);
	return (value);
}

bool	Config::extractAutoIndex(const std::string &locationBlock) {
	if (locationBlock.find("AutoIndex:") == std::string::npos) {
		return ("");
	}
	size_t start = locationBlock.find("AutoIndex:") + 11;
	size_t end = locationBlock.find(';', start);
	//if (locationBlock.find('\n', start) < locationBlock.find(';', start))
	//	throw MissingSemicolonException();
	std::string value = locationBlock.substr(start, end - start);
	if (value == "1" || value == "on" || value == "true")
		return (true);
	else
		return (false);
}

void	Config::splitLocationBlocks(std::vector<std::string> &locBlocks,
									const std::string &configBlock) {
	size_t	blockEnd = 0;
	size_t	blockStart = configBlock.find("location", blockEnd);
	size_t	numLocBlocks = 0;

	while (blockStart < configBlock.length() && blockEnd < configBlock.length()) {
		if (configBlock.find('}', blockStart) == std::string::npos) {
			throw (MissingClosingBracketException());
		}
		blockEnd = configBlock.find('}', blockStart);
		locBlocks.push_back(configBlock.substr(blockStart,
							(blockEnd - blockStart + 1)));
		blockStart = configBlock.find("location", blockEnd);
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

// todo:: is this the correct behaviour or should it just show 404 not found?
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
	validateDir("indexFile");
	validateDir("errorDirectory");
	validateDir("cgiDirectory");
	validateDir("uploadDirectory");
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

// todo: catch exceptions in main to correctly close program with bad config
void	Config::parseConfig(const std::string& configBlock) {
	populateGlobalVarsMap(configBlock);
	setLocations(configBlock);
//	validateNoEmptyEntry();
//	validateConfigDirs();
//	validateMethods();
//	validateHtml();
//	validatePort();
}
