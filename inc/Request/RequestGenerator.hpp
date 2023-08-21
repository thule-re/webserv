/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestGenerator.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 19:44:11 by treeps            #+#    #+#             */
/*   Updated: 2023/08/21 19:44:11 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_REQUESTGENERATOR_HPP
# define WEBSERV_REQUESTGENERATOR_HPP

# include <string>
# include "Socket/ClientSocket.hpp"
# include "Request/ARequest.hpp"
# include "Request/GETRequest.hpp"
# include "Request/POSTRequest.hpp"
# include "Request/DELETERequest.hpp"
# include "Request/InvalidRequest.hpp"

class RequestGenerator {
public:
	// constructors
	RequestGenerator();
	RequestGenerator(const RequestGenerator &);

	// destructor
	~RequestGenerator();

	// operator overload
	RequestGenerator &operator=(const RequestGenerator &);

	// member functions
	static ARequest *createRequest(const ClientSocket& clientSocket, const std::string& request);
};

#endif
