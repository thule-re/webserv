/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DELETERequest.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 16:11:53 by treeps            #+#    #+#             */
/*   Updated: 2023/08/21 16:11:53 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_DELETEREQUEST_HPP
# define WEBSERV_DELETEREQUEST_HPP

# include <iostream>
# include "ARequest.hpp"

class DELETERequest: public ARequest {
public:
	// constructors
	DELETERequest(const ClientSocket& clientSocket, const std::string& request);
	DELETERequest(const DELETERequest &);

	// destructor
	~DELETERequest();

	// operator overload
	DELETERequest &operator=(const DELETERequest &);

	// member functions
	Response handle();
private:
	// constructors
	DELETERequest();
};

#endif
