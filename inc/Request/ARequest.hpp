/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ARequest.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 16:08:40 by treeps            #+#    #+#             */
/*   Updated: 2023/08/21 16:08:40 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_AREQUEST_HPP
# define WEBSERV_AREQUEST_HPP

# include <string>
# include <fstream>
# include "Response/Response.hpp"
# include "Socket/ClientSocket.hpp"
# include "utils/utils.hpp"

class ARequest {
public:
	// constructors
	ARequest(const ClientSocket& clientSocket);
	ARequest(const ARequest &);

	// destructor
	virtual ~ARequest();

	// operator overload
	ARequest &operator=(const ARequest &);

	// member functions
	virtual Response handle() = 0;
	static ARequest *newRequest(const ClientSocket& clientSocket);

protected:
	// constructors
	ARequest();
	// member variables
	const ClientSocket _clientSocket;
	const std::string _request;
	std::string _method;

	// protected member functions
	std::string _extractPath(int start);
};

#endif
