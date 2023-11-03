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

# define BLUE "\x1B[34m"
# define RED "\x1B[31m"
# define RESET "\x1B[0m"

# include <string>
# include <iostream>
# include <fstream>
# include <vector>
# include <cstdlib>
# include <map>
# include "Parser/errorCodes.hpp"
# include "Parser/configStructs.hpp"
# include "Parser/ParserExceptions.hpp"
# include "utils/utils.hpp"

extern int	g_maxClients;
extern int	g_timeout;
extern int	g_maxFileSize;
extern int	g_verboseTrigger;

class Parser {
	public:
		// constructors
		Parser(int argc, char** argv);
		Parser(const Parser &other);

		// destructor
		~Parser();

		// operator overload
		Parser &operator=(const Parser &other);

		// exceptions: moved to ParserExceptions.hpp / ParserExceptions.cpp

		// member functions
		std::map<int, std::map<std::string, t_serverConfig> >&	getConfigMap();

	private:
		std::map<int, std::map<std::string, t_serverConfig> > _configMap;

		Parser();
		void	parseServerConfigs(std::string &rawConfig);

		static void	removeComments(std::string &fileContent);

		static void	checkInput(int argc, char** argv, std::string &pathToConfigFile);
		static void	readConfigFile(const std::string &pathToConfig, std::string &fileContent);

		// global variable parsing
		static void	parseGlobalVars(std::string &rawConfig);
		static void	extractTimeout(std::string &rawConfig);
		static void	extractMaxClients(std::string &rawConfig);
		static void	extractMaxFileSize(std::string &rawConfig);

		// server parsing
		static void			extractServerBlocks(std::vector<std::string> &serverBlocks,
										const std::string &rawConfig);
		static void				populateServerConfig(t_serverConfig &server, std::string &serverBlock);
		static std::string	extractServerValue(int num, const std::string& key, std::string &serverBlock);

		// location parsing
		static void				setLocations(t_serverConfig &serverConfig, const std::string &configBlock);
		static void			splitLocationBlocks(std::vector<std::string> &locationBlocks,
										const std::string &configBlock);
		static void				populateLocationConfig(t_locationConfig &locationConfig, std::string &locationBlock);
		static std::string	extractPath(const std::string &locationBlock);
		static bool			extractAutoIndex(const std::string &locationBlock);
		static std::string	extractLocationVariable(int varKey, const std::string &variable,
													const std::string &locationBlock);
};

#endif
