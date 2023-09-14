/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrautne <mtrautne@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 15:50:45 by mtrautne          #+#    #+#             */
/*   Updated: 2023/08/22 15:51:42 by mtrautne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser/Parser.hpp"

Parser::Parser() {}

Parser::Parser(const std::string &pathToConfig) {
	std::ifstream	configFile(pathToConfig);
	if (!configFile.is_open())
		throw CantOpenError();
	std::string	fileContent((std::istreambuf_iterator<char>(configFile)),
								(std::istreambuf_iterator<char>()));
	if (fileContent.empty())
		throw EmptyConfigFileError();
	parseConfig(fileContent);
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
const char *Parser::NoArgError::what() const _NOEXCEPT {
	return ("Parser initialised without config file");
}

const char *Parser::CantOpenError::what() const _NOEXCEPT {
	return ("Parser can't open config file");
}

const char *Parser::EmptyConfigFileError::what() const _NOEXCEPT {
	return ("Empty config file");
}

//member functions
std::vector<Config>&	Parser::getConfigArr() {
	return(_configArr);
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
//	std::cout << rawConfig << std::endl;
	for (size_t i = 0; i < serverBlocks.size(); i++) {
		Config	conf(serverBlocks[i]);
		_configArr.push_back(conf);
	}
}
