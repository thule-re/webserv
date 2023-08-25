/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 10:32:43 by treeps            #+#    #+#             */
/*   Updated: 2023/08/25 10:32:43 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HEADER_HPP
# define WEBSERV_HEADER_HPP

# include <string>
# include <sstream>
# include <iostream>
# include <map>
# include "utils/utils.hpp"

class Header {
public:
	// constructors
	Header();
	Header(const std::string &rawHeader);
	Header(const Header &);

	// destructor
	~Header();

	// operator overload
	Header &operator=(const Header &);
	std::string &operator[](const std::string &key);

	// member functions
	std::string getHeader(const std::string &key);

private:
	// member variables
	std::string _rawHeader;
	std::map<std::string, std::string> _headerMap;

	// member functions
	void _parseHeader();
	void _parseLine(const std::string &line);
	void _parseFirstLine(std::string &line);
};

#endif
