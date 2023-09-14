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

#include "Server/Cluster.hpp"

int main(int argc, char **argv) {
//	Server server;
//
//	if (argc == 5) {
//		std::cout << "debug" << std::endl;
//		server = Server(atoi(argv[1]), argv[2], argv[3], argv[4]);
//	} else {
//		server = Server();
//	}

	(void) argc;
	(void) argv;

	Cluster cluster = Cluster();

	cluster.initializeServers();
	cluster.loop();
	return (0);
}
