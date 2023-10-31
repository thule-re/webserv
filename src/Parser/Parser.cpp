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
	this->_configArr = other._configArr;
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
	extractMaxFileSize(rawConfig);
	std::cout << "timeout: " << g_timeout << " maxClients: " << g_maxClients
}

void	Parser::extractTimeout(std::string &rawConfig) {
	if (rawConfig.find("timeout:") == std::string::npos)
		return;
	size_t start = rawConfig.find("timeout:") + 9;
	size_t end = rawConfig.find(";\n", start);

	std::string timeoutStr = rawConfig.substr(start, end - start);
	g_timeout = atoi(timeoutStr.c_str());
	if (g_timeout < 1 || g_timeout > 1000)
		throw InvalidGlobalValueException();
}

void	Parser::extractMaxClients(std::string &rawConfig) {
	if (rawConfig.find("maxClients:") == std::string::npos)
		throw InvalidGlobalValueException();
	size_t start = rawConfig.find("maxClients:") + 11;
	size_t end = rawConfig.find(";\n", start);

	std::string maxClientsStr = rawConfig.substr(start, end - start);
	g_maxClients = atoi(maxClientsStr.c_str());
	if (g_maxClients < 1 || g_maxClients > 100000)
		throw InvalidGlobalValueException();
}

void	Parser::extractMaxFileSize(std::string &rawConfig) {
	if (rawConfig.find("maxFileSize:") == std::string::npos)
		throw InvalidGlobalValueException();
	size_t start = rawConfig.find("maxFileSize:") + 13;
	size_t end = rawConfig.find(";\n", start);

	std::string maxFileSize = rawConfig.substr(start, end - start);
	g_maxFileSize = atoi(maxFileSize.c_str());
	if (g_maxFileSize < 1 || g_maxFileSize > 2000000)
		throw InvalidGlobalValueException();
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
