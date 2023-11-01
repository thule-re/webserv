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
# include <map>
# include "Parser/errorCodes.hpp"

extern int	g_maxClients;
extern int	g_timeout;
extern int	g_maxFileSize;


typedef struct s_locationConfig {
	std::string	path;
	std::string	root;
	std::string	index;
	std::string	cgiExtension;
	std::string	upload;
	std::string	redirect;
	std::string	allowedMethods;
	bool		autoIndex;
}	t_locationConfig;

typedef struct s_serverConfig {
	int										port;
	std::string								serverName;
	std::string								errorDir;
	std::map<std::string, t_locationConfig>	locationMap;
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
		class InvalidConfigException: public std::exception {
		public:
			const char *what() const throw();
		};
		class ValueMissingException : public std::exception {
		public:
			virtual const char *what() const throw();
			ValueMissingException(const int &missingKey);
		private:
			int	_key;
		};
		class MissingSemicolonException : public std::exception {
		public:
			virtual const char* what() const throw();
		};
		class MissingClosingBracketException : public std::exception {
		public:
			virtual const char* what() const throw();
		};
		class InvalidLocationException : public std::exception {
		public:
			virtual const char* what() const throw();
		};
		// member functions
		std::map<int, std::map<std::string, t_serverConfig> >&	getConfigMap();

	private:
		std::map<int, std::map<std::string, t_serverConfig> > _configMap;

		Parser();
		void	parseServerConfigs(std::string &rawConfig);

		static void	removeComments(std::string &fileContent);

		static void	readConfigFile(const std::string &pathToConfig, std::string &fileContent);

		// global variable parsing
		static void	parseGlobalVars(std::string &rawConfig);
		static void	extractTimeout(std::string &rawConfig);
		static void	extractMaxClients(std::string &rawConfig);
		static void	extractMaxFileSize(std::string &rawConfig);

		// server parsing
		static void	extractServerBlocks(std::vector<std::string> &serverBlocks,
										const std::string &rawConfig);
		void		populateServerConfig(t_serverConfig &server, std::string &serverBlock);
		static void	setServerValue(int num, std::string &value,
									t_serverConfig &serverConfig, std::string &serverBlock);

		// location parsing
		void				setLocations(t_serverConfig &serverConfig, const std::string &configBlock);
		static void			splitLocationBlocks(std::vector<std::string> &locationBlocks,
										const std::string &configBlock);
		static void				populateLocationConfig(t_locationConfig &locationConfig, std::string &locationBlock);
		static std::string	extractPath(const std::string &locationBlock);
		static bool			extractAutoIndex(const std::string &locationBlock);
		static std::string	extractLocationVariable(const std::string &variable, const std::string &locationBlock);
};

#endif
