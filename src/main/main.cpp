/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 11:29:17 by treeps            #+#    #+#             */
/*   Updated: 2023/09/15 11:50:42 by mtrautne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server/Cluster.hpp"
#include "Parser/Parser.hpp"

int main(int argc, char **argv) {

	if (argc != 2) {
		std::cerr << "You need a config file, bozo!" << std::endl;
		return (1);
	}
	try {
		Parser parser(argv[1]);
		Cluster cluster(parser.getConfigArr());
		cluster.loop();
	}
	//catch block potentially needs work to properly free cluster stuff
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		return (1);
	}
	return (0);
}
