/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POSTRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tony <tony@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 16:11:41 by treeps            #+#    #+#             */
/*   Updated: 2023/08/26 13:54:37 by tony             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_POSTREQUEST_HPP
# define WEBSERV_POSTREQUEST_HPP

# include <iostream>
# include <fstream>
# include <fcntl.h>

# include "ARequest.hpp"
# define BUFFER_SIZE 1024
# define MAX_FILE_SIZE 2000000

class POSTRequest: public ARequest {
public:
	// constructors
	POSTRequest(ClientSocket *clientSocket);
	POSTRequest(const POSTRequest &other);

	// destructor
	~POSTRequest();

	// operator overload
	POSTRequest &operator=(const POSTRequest &);

	// member functions
	Response *handle();

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
};

#endif
