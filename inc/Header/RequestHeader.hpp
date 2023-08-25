/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHeader.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 10:32:43 by treeps            #+#    #+#             */
/*   Updated: 2023/08/25 10:32:43 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_REQUESTHEADER_HPP
# define WEBSERV_REQUESTHEADER_HPP

# include <string>
# include <sstream>
# include <iostream>
# include <map>
# include "Header/AHeader.hpp"
# include "utils/utils.hpp"

class RequestHeader: public AHeader {
public:
	// constructors
	RequestHeader();
	RequestHeader(const std::string &rawHeader);
	RequestHeader(const RequestHeader &);

	// destructor
	~RequestHeader();

	// operator overload
	RequestHeader &operator=(const RequestHeader &);

	// member functions
	std::string exportHeader();

private:
	// member functions
	void _parseHeader();
	void _parseLine(const std::string &line);
	void _parseFirstLine(std::string &line);
};

#endif
