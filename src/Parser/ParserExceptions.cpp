/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserExceptions.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrautne <mtrautne@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 19:56:19 by mtrautne          #+#    #+#             */
/*   Updated: 2023/11/03 19:56:19 by mtrautne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser/ParserExceptions.hpp"

const char *InvalidArgNumException::what() const throw() {
	return (RED "Error: Invalid arguments (or invalid number of arguments) "
				"in executable call." RESET);
}

const char *CantOpenConfigException::what() const throw() {
	return (RED "Error: Parser can't open config file." RESET);
}

const char *EmptyConfigFileException::what() const throw() {
	return (RED "Error: Parser detected empty config file." RESET);
}

const char *DuplicateServerNameException::what() const throw() {
	return (RED "Error: Parser detected duplicate port-serverName "
				"combination." RESET);
}

const char *InvalidGlobalValueException::what() const throw() {
	return (RED "Error: Global value out of range or otherwise invalid." RESET);
}

const char *InvalidPortException::what() const throw() {
	return (RED "Error: Port value out of acceptable range or missing." RESET);
}

const char *InvalidConfigException::what() const throw() {
	return (RED "Error: Config for at least one server is incomplete." RESET);
}

const char *MissingClosingBracketException::what() const throw() {
	return (RED "Error: Missing closing brackets for location in config file." RESET);
}

ValueMissingException::ValueMissingException(const int &missingKey)
		: _key(missingKey) {}


const char *ValueMissingException::what() const throw() {
	std::string	retString = RED "Error: At least one server directive in the config file "
								"is missing an instance of: " + getValStr(_key) + RESET;
	const char	*retStringC = retString.c_str();
	return (retStringC);
}

const char *MissingSemicolonException::what() const throw() {
	return (RED "Error: Missing semicolon in config." RESET);
}

const char *InvalidLocationException::what() const throw() {
	return (RED "Error: No, empty or invalid location block in config." RESET);
}

const char *InvalidPathException::what() const throw() {
	return (RED "Error: Invalid location path in config." RESET);
}

const char *NoValidServerConfigException::what() const throw() {
	return (RED "Error: No valid server block in config." RESET);
}

const char *ExceededMaxServerNumberException::what() const throw() {
	return (RED "Error: Too many servers in config. (max. 10)" RESET);
}

const char *ServerBlockSeparatorException::what() const throw() {
	return (RED "Error: Server block delimiter missing or incorrectly set up." RESET);
}

const char *EmptyServerBlockException::what() const throw() {
	return (RED "Error: Empty server block detected." RESET);
}

const char *ExceededMaxLocationNumberException::what() const throw() {
	return (RED "Error: Too many locations in at least one server block"
				". (max. 10)" RESET);
}

const char *DuplicateLocationNameException::what() const throw() {
	return (RED "Error: Duplicate location name detected." RESET);
}

const char *NoRootLocationException::what() const throw() {
	return (RED "Error: Root (location /) missing in at least one server block." RESET);
}
