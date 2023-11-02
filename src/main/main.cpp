/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrautne <mtrautne@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 11:29:17 by treeps            #+#    #+#             */
/*   Updated: 2023/09/28 11:45:54 by mtrautne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server/Cluster.hpp"
#include "Parser/Parser.hpp"
#include "utils/utils.hpp"

int	g_maxClients = 1000;
int	g_timeout = 1;
int	g_maxFileSize = 100000;

int main(int argc, char **argv) {
	std::string	pathToConfigFile;
	if (argc != 2)
		pathToConfigFile = "./config/webserv.conf";
	else
		pathToConfigFile = argv[1];
	try {
		Parser parser(pathToConfigFile);

		Cluster cluster(parser.getConfigMap());
		cluster.loop();
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		return (1);
	}
	return (0);
}
