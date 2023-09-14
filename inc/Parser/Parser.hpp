/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrautne <mtrautne@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 15:56:34 by mtrautne          #+#    #+#             */
/*   Updated: 2023/08/22 15:56:34 by mtrautne         ###   ########.fr       */
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
	class NoArgError : public std::exception {
	public:
		const char *what() const _NOEXCEPT;
	};
	class CantOpenError : public std::exception {
	public:
		const char *what() const _NOEXCEPT;
	};
	class EmptyConfigFileError: public std::exception {
	public:
		const char *what() const _NOEXCEPT;
	};

	// member functions
	std::vector<Config>&	getConfigArr();

	private:
		Parser();
		static void	parseConfig(std::string &rawConfig);
		std::vector<Config>	_configArr;
};

#endif
