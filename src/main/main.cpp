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

int main(int argc, char **argv) {
	Server server;

	if (argc == 5) {
		std::cout << "debug" << std::endl;
		server = Server(atoi(argv[1]), argv[2]);
	} else {
		server = Server();
	}
	server.init();
	server.loop();
	return (0);
}
