/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrautne <mtrautne@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 17:27:31 by mtrautne          #+#    #+#             */
/*   Updated: 2023/11/03 12:54:42 by mtrautne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server/Server.hpp"
#include "Parser/Parser.hpp"
#include "utils/utils.hpp"

int	g_timeout = 20;
int	g_maxClients = 200;
int	g_maxFileSize = 10000;

int main(int argc, char **argv) {
	std::string	pathToConfigFile;
	if (argc != 2)
		pathToConfigFile = "./config/webserv.conf";
	else
		pathToConfigFile = argv[1];
	try {
		Parser parser(pathToConfigFile);

		Server cluster(parser.getConfigMap());
		cluster.loop();
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		return (1);
	}
	return (0);
}
