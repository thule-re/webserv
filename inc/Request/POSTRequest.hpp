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
	POSTRequest(const ClientSocket &clientSocket);
	POSTRequest(const POSTRequest &other);

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
	std::string _fileData;
	std::string _filename;

	void _getFileData();
	void _getFilename();
	void _getBoundary();
	void _writeDataToOutfile();
	void _checkFilename();
	void _extractMultipartFormData();
};

#endif
