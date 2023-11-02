/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrautne <mtrautne@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 15:50:45 by mtrautne          #+#    #+#             */
/*   Updated: 2023/09/21 16:06:18 by mtrautne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser/Parser.hpp"

Parser::Parser() {}

Parser::Parser(const std::string &pathToConfig) {
	std::string fileContent;

	readConfigFile(pathToConfig, fileContent);
	removeComments(fileContent);
	parseGlobalVars(fileContent);
	parseServerConfigs(fileContent);
	printServerConfigMap(_configMap);
	// todo: Some test maybe idk.
	// checkForDuplicatePorts();
}

Parser::Parser(const Parser &other) {
	*this = other;
}

// destructor
Parser::~Parser() {
}

// operator overload
Parser &Parser::operator=(const Parser &other) {
	if (this == &other)
		return (*this);
	this->_configMap = other._configMap;
	return (*this);
}

// exceptions
const char *Parser::CantOpenConfigException::what() const throw() {
	return ("Error: Parser can't open config file.");
}

const char *Parser::EmptyConfigFileException::what() const throw() {
	return ("Error: Parser detected empty config file.");
}

const char *Parser::DuplicateServerNameException::what() const throw() {
	return ("Error: Parser detected duplicate config block.");
}

const char *Parser::InvalidGlobalValueException::what() const throw() {
	return ("Error: Global value out of range or otherwise invalid.");
}

const char *Parser::InvalidConfigException::what() const throw() {
	return ("Error: Config for at least one server is incomplete.");
}

const char *Parser::MissingClosingBracketException::what() const throw() {
	return ("Error: Missing closing brackets for location in config file.");
}

Parser::ValueMissingException::ValueMissingException(const int &missingKey)
		: _key(missingKey) {}

std::string	getValStr(const int& key) {
	switch (key) {
		case SERVERNAME:
			return "serverName";
		case PORT:
			return "port";
		case ERRORDIR:
			return ("ErrorDir");
		default:
			return "unspecificConfigValue";
	}
}

const char *Parser::ValueMissingException::what() const throw() {
	std::string	retString = "Error: No value found for: " + getValStr(_key);
	const char	*retStringC = retString.c_str();
	return (retStringC);
}

const char *Parser::MissingSemicolonException::what() const throw() {
	return ("Error: Missing semicolon in config.");
}

const char *Parser::InvalidLocationException::what() const throw() {
	return ("Error: No, empty or invalid location block in config.");
}

const char *Parser::InvalidPortException::what() const throw() {
	return ("Error: Port value out of acceptable range or missing.");
}

//getters
std::map<int, std::map<std::string, t_serverConfig> >&	Parser::getConfigMap() {
	return(_configMap);
}

//member functions
void	Parser::readConfigFile(const std::string &pathToConfig,
									std::string &fileContent) {
	std::ifstream	configFile(pathToConfig.c_str());
	if (!configFile.is_open())
		throw CantOpenConfigException();
	fileContent = std::string((std::istreambuf_iterator<char>(configFile)),
					(std::istreambuf_iterator<char>()));
	if (fileContent.empty())
		throw EmptyConfigFileException();
}

void	Parser::removeComments(std::string &fileContent) {
	size_t	endOfComment;
	size_t	startOfComment = std::min(std::min(fileContent.find("\n#", 0),
								fileContent.find(" #", 0)),
								fileContent.find("\t#", 0));
	while (startOfComment != std::string::npos) {
		endOfComment = fileContent.find('\n', startOfComment + 1);

		fileContent = fileContent.erase(startOfComment + 1, (endOfComment - startOfComment - 1));
		startOfComment = std::min(std::min(fileContent.find("\n#", startOfComment),
							fileContent.find(" #", startOfComment)),
							fileContent.find("\t#", startOfComment));
	}
}

void	Parser::parseGlobalVars(std::string &rawConfig) {
	extractTimeout(rawConfig);
	extractMaxClients(rawConfig);
	extractMaxFileSize(rawConfig);
}

void	Parser::extractTimeout(std::string &rawConfig) {
	if (rawConfig.find("timeout:") == std::string::npos)
		return;
	size_t start = rawConfig.find("timeout:") + 9;
	size_t end = rawConfig.find(";\n", start);

	std::string timeoutStr = rawConfig.substr(start, end - start);
	removeLeadingWhitespaces(timeoutStr);
	g_timeout = atoi(timeoutStr.c_str());
	if (g_timeout < 1 || g_timeout > 60)
		throw InvalidGlobalValueException();
}

void	Parser::extractMaxClients(std::string &rawConfig) {
	if (rawConfig.find("maxClients:") == std::string::npos)
		throw InvalidGlobalValueException();
	size_t start = rawConfig.find("maxClients:") + 11;
	size_t end = rawConfig.find(";\n", start);

	std::string maxClientsStr = rawConfig.substr(start, end - start);
	removeLeadingWhitespaces(maxClientsStr);
	g_maxClients = atoi(maxClientsStr.c_str());
	if (g_maxClients < 1 || g_maxClients > 1000)
		throw InvalidGlobalValueException();
}

void	Parser::extractMaxFileSize(std::string &rawConfig) {
	if (rawConfig.find("maxFileSize:") == std::string::npos)
		throw InvalidGlobalValueException();
	size_t start = rawConfig.find("maxFileSize:") + 13;
	size_t end = rawConfig.find(";\n", start);

	std::string maxFileSize = rawConfig.substr(start, end - start);
	removeLeadingWhitespaces(maxFileSize);
	g_maxFileSize = atoi(maxFileSize.c_str());
	if (g_maxFileSize < 1 || g_maxFileSize > 100000)
		throw InvalidGlobalValueException();
}

void	Parser::parseServerConfigs(std::string &rawConfig) {
	std::vector<std::string> serverBlocks;

	extractServerBlocks(serverBlocks, rawConfig);
	for (size_t i = 0; i < serverBlocks.size(); i++) {
		if (i == 0) {
			t_serverConfig	defaultServerConfig;
			populateServerConfig(defaultServerConfig, serverBlocks[i]);
			defaultServerConfig.serverName = "default";
			_configMap[defaultServerConfig.port][defaultServerConfig.serverName] = defaultServerConfig;
		}
		t_serverConfig	serverConfig;
		populateServerConfig(serverConfig, serverBlocks[i]);
		if (_configMap[serverConfig.port].count(serverConfig.serverName) > 0) {
			throw DuplicateServerNameException();
		}
		else
			_configMap[serverConfig.port][serverConfig.serverName] = serverConfig;
	}
}

void	Parser::extractServerBlocks(std::vector<std::string> &serverBlocks, const std::string &rawConfig) {
	size_t start = 0, end;
	while((end = rawConfig.find("</server>", start)) != std::string::npos) {
		serverBlocks.push_back(rawConfig.substr(start, end - start));
		start = end + 10;
		while ((start < rawConfig.length()) && (rawConfig[start] == ' '
												|| rawConfig[start] == '\n'))
			start++;
	}
	if (serverBlocks.empty())
		throw InvalidConfigException();
}

void	Parser::populateServerConfig(t_serverConfig &serverConfig, std::string &serverBlock) {
	serverConfig.serverName = extractServerValue(SERVERNAME, "serverName:", serverBlock);
	serverConfig.errorDir = extractServerValue(ERRORDIR, "errorDir:", serverBlock);
	serverConfig.port = atoi(extractServerValue(PORT, "port:", serverBlock).c_str());
	if (serverConfig.port < 0 || serverConfig.port > 10000)
		throw InvalidPortException();
	setLocations(serverConfig, serverBlock);
}

std::string Parser::extractServerValue(int num, std::string key, std::string &serverBlock) {
	size_t		valStart;
	size_t		valEnd;
	std::string	value;

	if (serverBlock.find(key) == std::string::npos)
		throw ValueMissingException(num);
	else
		valStart = serverBlock.find(key);
	valStart = serverBlock.find(':', valStart) + 2;
	valEnd = valStart;
	while (valEnd < serverBlock.length() && serverBlock[valEnd + 1] != ';'
		   && serverBlock[valEnd + 1] != '\n')
		valEnd++;
	value = serverBlock.substr(valStart, (valEnd - valStart + 1));
	removeLeadingWhitespaces(value);
	if (value.empty())
		throw InvalidConfigException();
	if (serverBlock[valEnd] == '\n') {
		throw MissingSemicolonException();
	}
	return (value);
}

void	Parser::setLocations(t_serverConfig &serverConfig, const std::string &configBlock) {
	std::vector<std::string>	locationBlocks;

	splitLocationBlocks(locationBlocks, configBlock);
	if (locationBlocks.empty())
		throw InvalidLocationException();
	for (size_t i = 0; i < locationBlocks.size(); i++) {
		t_locationConfig	locationConfig;
		populateLocationConfig(locationConfig, locationBlocks[i]);
		serverConfig.locationMap[locationConfig.path] = locationConfig;
	}
}

void	Parser::splitLocationBlocks(std::vector<std::string> &locationBlocks,
									const std::string &configBlock) {
	size_t	blockEnd = 0;
	size_t	blockStart = configBlock.find("location", blockEnd);
	size_t	numLocBlocks = 0;

	while (blockStart < configBlock.length() && blockEnd < configBlock.length()) {
		if (configBlock.find('}', blockStart) == std::string::npos ||
			configBlock.find('}', blockStart) >
			configBlock.find("location", blockStart + 9)) {
			throw (MissingClosingBracketException());
		}
		blockEnd = configBlock.find('}', blockStart);
		locationBlocks.push_back(configBlock.substr(blockStart,
											   (blockEnd - blockStart + 1)));
		blockStart = configBlock.find("location", blockEnd);
		numLocBlocks++;
	}
}

void	Parser::populateLocationConfig(t_locationConfig &locationConfig, std::string &locationBlock) {
	locationConfig.path = extractPath(locationBlock);
	locationConfig.autoIndex = extractAutoIndex(locationBlock);
	locationConfig.root = extractLocationVariable("root:", locationBlock);
	locationConfig.index = extractLocationVariable("index:", locationBlock);
	locationConfig.cgiExtension = extractLocationVariable("cgiExtension:", locationBlock);
	locationConfig.upload = extractLocationVariable("uploadDir:", locationBlock);
	locationConfig.redirect = extractLocationVariable("httpRedir:", locationBlock);
	locationConfig.allowedMethods = extractLocationVariable("methods:", locationBlock);
}

std::string	Parser::extractPath(const std::string &locationBlock) {
	if (locationBlock.find("location") == std::string::npos)
		return ("");
	size_t start = locationBlock.find("location") + 9;
	size_t end = locationBlock.find('{', start);
	std::string value = locationBlock.substr(start, end - start - 1);
	removeLeadingWhitespaces(value);
	return (value);
}

std::string	Parser::extractLocationVariable(const std::string &variable, const std::string &locationBlock) {
	if (locationBlock.find(variable) == std::string::npos)
		return ("");
	size_t start = locationBlock.find(variable) + variable.size();
	size_t end = locationBlock.find(';', start);
	if (locationBlock.find('\n', start) < locationBlock.find(';', start))
		throw MissingSemicolonException();
	std::string value = locationBlock.substr(start, end - start);
	removeLeadingWhitespaces(value);
	return (value);
}

bool	Parser::extractAutoIndex(const std::string &locationBlock) {
	if (locationBlock.find("AutoIndex:") == std::string::npos) {
		return ("");
	}
	size_t start = locationBlock.find("AutoIndex:") + 11;
	size_t end = locationBlock.find(';', start);
	if (locationBlock.find('\n', start) < locationBlock.find(';', start))
		throw MissingSemicolonException();
	std::string value = locationBlock.substr(start, end - start);
	removeLeadingWhitespaces(value);
	if (value == "1" || value == "on" || value == "true")
		return (true);
	else
		return (false);
}
