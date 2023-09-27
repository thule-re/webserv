/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrautne <mtrautne@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 15:56:34 by mtrautne          #+#    #+#             */
/*   Updated: 2023/09/15 11:39:39 by mtrautne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include <string>
# include <iostream>
# include <fstream>
# include <vector>
# include "Parser/Config.hpp"
# include "Parser/errorCodes.hpp"

class Parser {
	public:
	// constructors
	Parser(const std::string &pathToConfig);
	Parser(const Parser & other);

	// destructor
	~Parser();

	// operator overload
	Parser &operator=(const Parser &other);

// exceptions
	class NoArgException : public std::exception {
	public:
		const char *what() const throw();
	};
	class CantOpenException : public std::exception {
	public:
		const char *what() const throw();
	};
	class EmptyConfigFileException: public std::exception {
	public:
		const char *what() const throw();
	};
	class DuplicateConfigException: public std::exception {
	public:
		const char *what() const throw();
	};

	// member functions
	std::vector<Config>&	getConfigArr();

	private:
		Parser();
		static void	removeComments(std::string &fileContent);
		void	parseConfig(std::string &rawConfig);
		void	checkForDuplicateServerConfigs();
		std::vector<Config>	_configArr;
};

#endif
