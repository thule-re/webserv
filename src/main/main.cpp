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
#include "Parser/Parser.hpp"

int main(int argc, char **argv) {

	if (argc != 2) {
		std::cerr << "You need a config file, bozo!" << std::endl;
		return (1);
	}

	Parser parser(argv[1]);

	Cluster cluster(parser.getConfigArr());

	cluster.loop();
	return (0);
}
