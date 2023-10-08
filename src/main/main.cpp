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

int main(int argc, char **argv) {
	std::string	pathToConfigFile;
	if (argc != 2)
		pathToConfigFile = "./config/webserv.conf";
	else
		pathToConfigFile = argv[1];
	try {
		Parser parser(pathToConfigFile);
		std::cout << parser << std::endl;
		Cluster cluster(parser.getConfigArr());
		cluster.loop();
	}
	//todo: catch block potentially needs work to properly free cluster stuff
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		return (1);
	}
	return (0);
}
