/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrautne <mtrautne@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 15:50:45 by mtrautne          #+#    #+#             */
/*   Updated: 2023/09/15 11:53:32 by mtrautne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser/Parser.hpp"

Parser::Parser() {}

Parser::Parser(const std::string &pathToConfig) {
	std::ifstream	configFile(pathToConfig);
	if (!configFile.is_open())
		throw CantOpenException();
	std::string	fileContent((std::istreambuf_iterator<char>(configFile)),
								(std::istreambuf_iterator<char>()));
	if (fileContent.empty())
		throw EmptyConfigFileException();
	removeComments(fileContent);
	parseConfig(fileContent);
	checkForDuplicateServerConfigs();
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
	return (*this);
}

// exceptions
const char *Parser::NoArgException::what() const _NOEXCEPT {
	return ("Error: Parser initialised without config file path argument.");
}

const char *Parser::CantOpenException::what() const _NOEXCEPT {
	return ("Error: Parser can't open config file.");
}

const char *Parser::EmptyConfigFileException::what() const _NOEXCEPT {
	return ("Error: Parser detected empty config file.");
}

const char *Parser::DuplicateConfigException::what() const _NOEXCEPT {
	return ("Error: Parser detected duplicate config.");
}

//member functions
std::vector<Config>&	Parser::getConfigArr() {
	return(_configArr);
}

void	Parser::removeComments(std::string &fileContent) {
	size_t	endComment;
	size_t	startComment = std::min(std::min(fileContent.find("\n#", 0),
				fileContent.find(" #", 0)), fileContent.find("\t#", 0));
	while (startComment != std::string::npos) {
		endComment = fileContent.find('\n', startComment + 1);

		fileContent = fileContent.erase(startComment + 1, (endComment - startComment - 1));
		startComment = std::min(std::min(fileContent.find("\n#", startComment),
			fileContent.find(" #", startComment)), fileContent.find("\t#", startComment));
	}
}

void	Parser::parseConfig(std::string &rawConfig) {
	std::vector<std::string> serverBlocks;
	size_t start = 0, end;
	while((end = rawConfig.find("</server>", start)) != std::string::npos) {
		serverBlocks.push_back(rawConfig.substr(start, end - start));
		start = end + 10;
		while ((start < rawConfig.length()) && (rawConfig[start] == ' '
				|| rawConfig[start] == '\n'))
			start++;
	}
	for (size_t i = 0; i < serverBlocks.size(); i++) {
		Config	conf(serverBlocks[i]);
		_configArr.push_back(conf);
	}
}

void	Parser::checkForDuplicateServerConfigs() {
	std::vector<std::string> uniqueConfigs;
	for (size_t i = 0; i < _configArr.size(); i++) {
		std::string configIdent = _configArr[i].getMap()["serverName"]
								+ _configArr[i].getMap()["port"];
		for (size_t j = 0; j < uniqueConfigs.size(); j++) {
			if (configIdent == uniqueConfigs[j])
				throw DuplicateConfigException();
		}
		uniqueConfigs.push_back(configIdent);
	}
}
