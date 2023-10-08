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
	checkForDuplicatePorts();
}

Parser::Parser(const Parser &other) :
_timeout(), _maxClients(), _bufferSize(), _maxEvents(), _backlog() {
	*this = other;
}

// destructor
Parser::~Parser() {
}

// operator overload
Parser &Parser::operator=(const Parser &other) {
	if (this == &other)
		return (*this);
	this->_configArr = other._configArr;
	this->_timeout = other._timeout;
	this->_maxClients = other._maxClients;
	this->_bufferSize = other._bufferSize;
	this->_maxEvents = other._maxEvents;
	this->_backlog = other._backlog;
	return (*this);
}

// exceptions
const char *Parser::CantOpenException::what() const throw() {
	return ("Error: Parser can't open config file.");
}

const char *Parser::EmptyConfigFileException::what() const throw() {
	return ("Error: Parser detected empty config file.");
}

const char *Parser::DuplicateConfigException::what() const throw() {
	return ("Error: Parser detected duplicate config.");
}

const char *Parser::InvalidGlobalValueException::what() const throw() {
	return ("Error: Global Value missing or invalid.");
}

//getters
std::vector<Config>&	Parser::getConfigArr() {
	return(_configArr);
}

int	Parser::getTimeout() const {
	return (_timeout);
}

int	Parser::getMaxClients() const {
	return (_maxClients);
}

int	Parser::getBufferSize() const {
	return (_bufferSize);
}

int	Parser::getMaxEvents() const {
	return (_maxEvents);
}

int	Parser::getBacklog() const {
	return (_backlog);
}

//member functions
void	Parser::readConfigFile(const std::string &pathToConfig,
									std::string &fileContent) {
	std::ifstream	configFile(pathToConfig.c_str());
	if (!configFile.is_open())
		throw CantOpenException();
	fileContent = std::string((std::istreambuf_iterator<char>(configFile)),
					(std::istreambuf_iterator<char>()));
	if (fileContent.empty())
		throw EmptyConfigFileException();
}

void	Parser::removeComments(std::string &fileContent) {
	size_t	endOfComment;
	size_t	startOfComment = std::min(std::min(fileContent.find("\n#", 0),fileContent.find(" #", 0)),
										fileContent.find("\t#", 0));
	while (startOfComment != std::string::npos) {
		endOfComment = fileContent.find('\n', startOfComment + 1);

		fileContent = fileContent.erase(startOfComment + 1, (endOfComment - startOfComment - 1));
		startOfComment = std::min(std::min(fileContent.find("\n#", startOfComment),
										   fileContent.find(" #", startOfComment)), fileContent.find("\t#", startOfComment));
	}
}

void	Parser::parseServerConfigs(std::string &rawConfig) {
	std::vector<std::string> serverBlocks;

	extractServerBlocks(serverBlocks, rawConfig);
	for (size_t i = 0; i < serverBlocks.size(); i++) {
		Config	conf(serverBlocks[i]);
		_configArr.push_back(conf);
	}
}

void	Parser::parseGlobalVars(std::string &rawConfig) {
	extractTimeout(rawConfig);
	extractMaxClients(rawConfig);
	extractBufferSize(rawConfig);
	extractMaxEvents(rawConfig);
	extractBacklog(rawConfig);
}

void	Parser::extractTimeout(std::string &rawConfig) {
	if (rawConfig.find("timeout:") == std::string::npos)
		throw InvalidGlobalValueException();
	size_t start = rawConfig.find("timeout:") + 9;
	size_t end = rawConfig.find(";\n", start);

	std::string timeoutStr = rawConfig.substr(start, end - start);
	int	timeout = atoi(timeoutStr.c_str());
	if (timeout < 1 || timeout > 1000)
		throw InvalidGlobalValueException();
	_timeout = timeout;
}

void	Parser::extractMaxClients(std::string &rawConfig) {
	if (rawConfig.find("maxClients:") == std::string::npos)
		throw InvalidGlobalValueException();
	size_t start = rawConfig.find("maxClients:") + 11;
	size_t end = rawConfig.find(";\n", start);

	std::string maxClientsStr = rawConfig.substr(start, end - start);
	int	maxClients = atoi(maxClientsStr.c_str());
	if (maxClients < 1 || maxClients > 100000)
		throw InvalidGlobalValueException();
	_maxClients = maxClients;
}

void	Parser::extractBufferSize(std::string &rawConfig) {
	if (rawConfig.find("bufferSize:") == std::string::npos)
		throw InvalidGlobalValueException();
	size_t start = rawConfig.find("bufferSize:") + 11;
	size_t end = rawConfig.find(";\n", start);

	std::string bufferSizeStr = rawConfig.substr(start, end - start);
	int	bufferSize = atoi(bufferSizeStr.c_str());
	if (bufferSize < 1 || bufferSize > 100000)
		throw InvalidGlobalValueException();
	_bufferSize = bufferSize;
}

void	Parser::extractMaxEvents(std::string &rawConfig) {
	if (rawConfig.find("maxEvents:") == std::string::npos)
		throw InvalidGlobalValueException();
	size_t start = rawConfig.find("maxEvents:") + 10;
	size_t end = rawConfig.find(";\n", start);

	std::string maxEventsStr = rawConfig.substr(start, end - start);
	int	maxEvents = atoi(maxEventsStr.c_str());
	if (maxEvents < 1 || maxEvents > 10000)
		throw InvalidGlobalValueException();
	_maxEvents = maxEvents;
}

void	Parser::extractBacklog(std::string &rawConfig) {
	if (rawConfig.find("backlog:") == std::string::npos)
		throw InvalidGlobalValueException();
	size_t start = rawConfig.find("backlog:") + 8;
	size_t end = rawConfig.find(";\n", start);

	std::string backlogStr = rawConfig.substr(start, end - start);
	int	backlog = atoi(backlogStr.c_str());
	if (backlog < 1 || backlog > 1000)
		throw InvalidGlobalValueException();
	_backlog = backlog;
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
}

void	Parser::checkForDuplicatePorts() {
	std::vector<std::string> uniquePorts;
	for (size_t i = 0; i < _configArr.size(); i++) {
		std::string port = _configArr[i].getMap()["port"];
		for (size_t j = 0; j < uniquePorts.size(); j++) {
			if (port == uniquePorts[j])
				throw DuplicateConfigException();
		}
		uniquePorts.push_back(port);
	}
}

std::ostream& operator<<(std::ostream& output, const Parser& object) {
	output << "Parser global Vars: timeout:" << object.getTimeout()
		<< "| maxClients:" << object.getMaxClients()
		<< "| bufferSize:" << object.getBufferSize()
		<< "| maxEvents:" << object.getMaxEvents()
		<< "| backlog:" << object.getBacklog() << "|" << std::endl;
	return (output);
}
