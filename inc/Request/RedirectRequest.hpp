/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirectRequest.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 17:15:17 by treeps            #+#    #+#             */
/*   Updated: 2023/09/15 17:15:17 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_REDIRECTREQUEST_HPP
# define WEBSERV_REDIRECTREQUEST_HPP

# include "Request/ARequest.hpp"

class RedirectRequest: public ARequest {
public:
	// constructors
	RedirectRequest(ClientSocket* clientSocket);
	RedirectRequest(const RedirectRequest &);

	// destructor
	~RedirectRequest();

	// operator overload
	RedirectRequest &operator=(const RedirectRequest &);

	// member functions
	Response *handle();

private:
	RedirectRequest();

};

#endif
