/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrautne <mtrautne@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 15:56:34 by mtrautne          #+#    #+#             */
/*   Updated: 2023/10/19 16:44:49 by mtrautne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include <string>
# include <iostream>
# include <fstream>
# include <vector>
# include <cstdlib>
# include "Parser/Config.hpp"
# include "Parser/errorCodes.hpp"

extern int	g_bufferSize;
extern int	g_maxClients;
extern int	g_timeout;
extern int	g_maxFileSize;

class Parser {
	public:
		// constructors
		Parser(const std::string &pathToConfig);
		Parser(const Parser &other);

		// destructor
		~Parser();

		// operator overload
		Parser &operator=(const Parser &other);

		// exceptions
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
		class InvalidGlobalValueException: public std::exception {
		public:
			const char *what() const throw();
		};

		// member functions
		std::vector<Config>&	getConfigArr();

	private:
		std::vector<Config>	_configArr;

		Parser();
		void	parseServerConfigs(std::string &rawConfig);
		void	checkForDuplicatePorts();

		static void	readConfigFile(const std::string &pathToConfig,
											std::string &fileContent);
		static void	removeComments(std::string &fileContent);
		static void	extractServerBlocks(std::vector<std::string> &serverBlocks,
										const std::string &rawConfig);
		void	parseGlobalVars(std::string &rawConfig);

		void	extractTimeout(std::string &rawConfig);
		void	extractMaxClients(std::string &rawConfig);
		void	extractBufferSize(std::string &rawConfig);
		void	extractMaxFileSize(std::string &rawConfig);
};

#endif
