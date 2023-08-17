/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 11:29:03 by treeps            #+#    #+#             */
/*   Updated: 2023/08/17 11:29:03 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// constructors
Server::Server() {
	std::cout << "Server default constructor called" << std::endl;
}

Server::Server(const Server &other) {
	std::cout << "Server copy constructor called" << std::endl;
	*this = other;
}

// destructor
Server::~Server() {
	std::cout << "Server destructor called" << std::endl;
}

// operator overload
Server &Server::operator=(const Server &other) {

	std::cout << "Server assignment operator called" << std::endl;
	if (this == &other)
		return (*this);

	return (*this);
}
