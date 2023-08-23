/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POSTRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 16:11:41 by treeps            #+#    #+#             */
/*   Updated: 2023/08/21 16:11:41 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_POSTREQUEST_HPP
# define WEBSERV_POSTREQUEST_HPP

# include <iostream>
# include <fstream>
# include "ARequest.hpp"

class POSTRequest: public ARequest {
public:
	// constructors
	POSTRequest(const ClientSocket& clientSocket, const std::string& request);
	POSTRequest(const POSTRequest &);

	// destructor
	~POSTRequest();

	// operator overload
	POSTRequest &operator=(const POSTRequest &);

	// member functions
	Response handle();

private:
	// constructors
	POSTRequest();
	std::string _boundary;
	std::string _requestData;
	std::string _fileData;

	std::string extractFileName(const std::string &request);
	std::string stripHeaderFromRequest(const std::string &request);
	std::string extractMultipartFormData();
	std::string getBoundary();
	void writeDataToOutfile(std::string fileData);
};

#endif
