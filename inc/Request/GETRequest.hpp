/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GETRequest.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 16:11:27 by treeps            #+#    #+#             */
/*   Updated: 2023/08/21 16:11:27 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_GETREQUEST_HPP
# define WEBSERV_GETREQUEST_HPP

# include <iostream>
# include <dirent.h>
# include "ARequest.hpp"

class GETRequest: public ARequest {
public:
	// constructors
	GETRequest(ClientSocket* clientSocket);
	GETRequest(const GETRequest &);

	// destructor
	~GETRequest();

	// operator overload
	GETRequest &operator=(const GETRequest &);

	// member functions
	Response *handle();

private:
	// constructors
	GETRequest();

	// member functions
	std::string _getDirectoryListing(const std::string& path);
};

#endif
