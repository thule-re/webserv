/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrautne <mtrautne@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 09:40:26 by mtrautne          #+#    #+#             */
/*   Updated: 2023/10/06 21:12:03 by mtrautne         ###   ########.fr       */
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
		case PORT:
			return "port";
		case TIMEOUT:
			return "timeout";
		case MAXCLIENTS:
			return "maxClients";
		case BUFFERSIZE:
			return "bufferSize";
		case MAXEVENTS:
			return "maxEvents";
		case BACKLOG:
			return "backlog";
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

const char *Config::MissingSemicolonException::what() const throw() {
	return ("Error: Missing semicolon in config.");
}

const char *Config::NoValidMethodException::what() const throw() {
	return ("Error: No valid method found for server block in config.");
}

const char *Config::InvalidPortException::what() const throw() {
	return ("Error: Invalid port in config file.");
}

const char *Config::MissingClosingBracketException::what() const throw() {
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
	std::string GlobalVarsBlock;
	std::string serverVarsBlock = extractServerVarsBlock(configBlock);

	setServerValue()
	setServerValue(SERVERNAME, configBlock);
	setServerValue(PORT, configBlock);
	setServerValue(HTML, configBlock);
	setServerValue(METHODS, configBlock);
}

void	Config::setServerValue(const int key, const std::string &configBlock) {
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
	size_t end = locationBlock.find(';', start);
	if (locationBlock.find('\n', start) < locationBlock.find(';', start))
		throw MissingSemicolonException();
	std::string value = locationBlock.substr(start, end - start);
	return (value);
}

std::string	Config::extractMethods(const std::string &locationBlock) {
	if (locationBlock.find("methods:") == std::string::npos)
		return ("");
	size_t start = locationBlock.find("methods:") + 9;
	size_t end = locationBlock.find(';', start);
	if (locationBlock.find('\n', start) < locationBlock.find(';', start))
		throw MissingSemicolonException();
	std::string value = locationBlock.substr(start, end - start);
	replaceAll(value, ", ", "");
	return (value);
}

std::string	Config::extractPath(const std::string &locationBlock) {
	if (locationBlock.find("location") == std::string::npos)
		return ("");
	size_t start = locationBlock.find("location") + 9;
	size_t end = locationBlock.find('{', start);
	if (locationBlock.find('\n', start) < locationBlock.find(';', start))
		throw MissingSemicolonException();
	std::string value = locationBlock.substr(start, end - start - 1);
	return (value);
}

std::string	Config::extractRoot(const std::string &locationBlock) {
	if (locationBlock.find("root:") == std::string::npos)
		return ("");
	size_t start = locationBlock.find("root:") + 6;
	size_t end = locationBlock.find(';', start);
	 if (locationBlock.find('\n', start) < locationBlock.find(';', start))
	 	throw MissingSemicolonException();
	std::string value = locationBlock.substr(start, end - start);
	return (value);
}

std::string	Config::extractIndex(const std::string &locationBlock) {
	if (locationBlock.find("index:") == std::string::npos)
		return ("");
	size_t start = locationBlock.find("index:") + 7;
	size_t end = locationBlock.find(';', start);
	 if (locationBlock.find('\n', start) < locationBlock.find(';', start))
	 	throw MissingSemicolonException();
	std::string value = locationBlock.substr(start, end - start);
	return (value);
}

std::string	Config::extractCgi(const std::string &locationBlock) {
	if (locationBlock.find("cgiDir:") == std::string::npos)
		return ("");
	size_t start = locationBlock.find("cgiDir:") + 8;
	size_t end = locationBlock.find(';', start);
	if (locationBlock.find('\n', start) < locationBlock.find(';', start))
		throw MissingSemicolonException();
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
	if (locationBlock.find('\n', start) < locationBlock.find(';', start))
		throw MissingSemicolonException();
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
	_configMap["numLocBlocks"] = ss.str()
}

void	Config::validatePort() {
	for (size_t i = 0; i < _configMap["port"].length(); i++) {
		if (!std::isdigit(_configMap["port"][i]))
			throw InvalidPortException();
	}
	std::istringstream ss(_configMap["port"]);
	int	portInt;
	ss >> portInt;
	if (portInt < 0 || portInt > 65535)
		throw InvalidPortException();
}

void	Config::parseConfig(const std::string& configBlock) {
	populateGlobalVarsMap(configBlock);
	setLocations(configBlock);
	validatePort();
}
