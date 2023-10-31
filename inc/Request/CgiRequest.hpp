/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiRequest.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 11:27:17 by treeps            #+#    #+#             */
/*   Updated: 2023/08/30 11:27:17 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_CGIREQUEST_HPP
# define WEBSERV_CGIREQUEST_HPP

# include "Request/ARequest.hpp"
# include "Response/Response.hpp"
# include <cstring> // todo: replace bzero wit c++98 conform function
# include <cstdio>
# include <csignal>
# include <vector>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/types.h>

extern char **environ;

class CgiRequest: public ARequest {
public:
	// constructors
	CgiRequest(ClientSocket *clientSocket);
	CgiRequest(const CgiRequest &);

	// destructor
	~CgiRequest();

	// operator overload
	CgiRequest &operator=(const CgiRequest &);

	// member functions
	Response *handle();

private:
	// constructors
	CgiRequest();

	// member functions
	void _getScriptPath();
	void _getQueryString();
	void _getPathInfo();
	void _writeCgiInput();
	void _readCgiOutput();
	void _setEnv();
	void _exportEnv();
	void _execCgi(Response *response);

	// member variables
	char **_envp;
	std::vector<std::string>_env;
	std::string _scriptPath;
	std::string _queryString;
	std::string _pathInfo;
	std::string _boundary;

	std::string _cgiOutputString;

	int _cgiInput[2];
	int _cgiOutput[2];

};

#endif
