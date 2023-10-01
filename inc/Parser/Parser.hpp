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
# include <cstdlib>
# include "Parser/Config.hpp"
# include "Parser/errorCodes.hpp"

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
		int						getTimeout();
		int						getMaxClients();
		int						getBufferSize();
		int						getMaxEvents();
		int						getBacklog();

	private:
		std::vector<Config>	_configArr;
		int					_timeout;
		int					_maxClients;
		int					_bufferSize;
		int					_maxEvents;
		int					_backlog;

		Parser();
		void	parseServerConfigs(std::string &rawConfig);
		void	checkForDuplicatePorts();

		static void	readConfigFile(const std::string &pathToConfig,
											std::string &fileContent);
		static void			removeComments(std::string &fileContent);
		static void			extractServerBlocks(std::vector<std::string> &serverBlocks,
													const std::string &rawConfig);
		void	parseGlobalVars(std::string &rawConfig);

		void	extractTimeout(std::string &rawConfig);
		void	extractMaxClients(std::string &rawConfig);
		void	extractBufferSize(std::string &rawConfig);
		void	extractMaxEvents(std::string &rawConfig);
		void	extractBacklog(std::string &rawConfig);
};

#endif
