/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 11:29:17 by treeps            #+#    #+#             */
/*   Updated: 2023/08/17 11:29:17 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server/Server.hpp"
#include "Parser/Parser.hpp"

int main(int argc, char **argv) {
	Server server;

	if (argc != 2) {
		std::cerr << "Invalid amount of arguments. Needs 1, has "
				  << argc - 1 << std::endl;
		return (1);
	}
	Parser	parser(argv[1]);
	std::cout << parser.getConfigArr()[0].getMap()["root"] << std::endl;
//	if (argc == 5) {
//		std::cout << "debug" << std::endl;
//		server = Server(atoi(argv[1]), argv[2], argv[3], argv[4]);
//	} else {
//		server = Server();
//	}
//	server.init();
//	server.loop();
	return (0);
}
