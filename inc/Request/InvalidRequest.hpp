/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InvalidRequest.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 16:57:40 by treeps            #+#    #+#             */
/*   Updated: 2023/08/21 16:57:40 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_INVALIDREQUEST_HPP
# define WEBSERV_INVALIDREQUEST_HPP

# include "Request/ARequest.hpp"

class InvalidRequest : public ARequest {
public:
	// constructors
	InvalidRequest(const ClientSocket& clientSocket, const std::string& request);
	InvalidRequest(const ClientSocket &clientSocket, const std::string &request, int code);
	InvalidRequest(const InvalidRequest &);


// destructor
	~InvalidRequest();

	// operator overload
	InvalidRequest &operator=(const InvalidRequest &);

	// member functions
	Response handle();

private:
	// constructors
	InvalidRequest();

	// member variables
	int _code;
};

#endif
