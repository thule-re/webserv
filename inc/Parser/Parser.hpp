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

extern int	g_maxClients;
extern int	g_timeout;
extern int	g_maxFileSize;


typedef struct s_locationConfig {
	std::string	path;
	std::string	root;
	std::string	index;
	std::string	cgi;
	std::string	upload;
	std::string	tryFiles;
	std::string	redirect;
	std::string	allowedMethods;
	bool		autoIndex;
}	t_locationConfig;

typedef struct s_serverConfig {
	int port;
	std::string serverName;
	std::map<std::string, t_locationConfig> t_locationMap;
}	t_serverConfig;

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
		class CantOpenConfigException : public std::exception {
		public:
			const char *what() const throw();
		};
		class EmptyConfigFileException: public std::exception {
		public:
			const char *what() const throw();
		};
		class DuplicateServerNameException: public std::exception {
		public:
			const char *what() const throw();
		};
		class InvalidGlobalValueException: public std::exception {
		public:
			const char *what() const throw();
		};

		// member functions
		std::map<int, std::map<std::string, t_serverConfig> >&	getConfigMap();

	private:
		std::map<int, std::map<std::string, t_serverConfig> > _configMap;

		Parser();
		void	parseServerConfigs(std::string &rawConfig);
		void	checkForDuplicatePorts();

		static void	readConfigFile(const std::string &pathToConfig,
											std::string &fileContent);
		static void	removeComments(std::string &fileContent);
		static void	parseGlobalVars(std::string &rawConfig);
		static void	extractTimeout(std::string &rawConfig);
		static void	extractMaxClients(std::string &rawConfig);
		static void	extractMaxFileSize(std::string &rawConfig);
		static void	extractServerBlocks(std::vector<std::string> &serverBlocks,
										const std::string &rawConfig);
		static void	populateServer(t_serverConfig &server, std::string &serverBlock);
};

#endif
