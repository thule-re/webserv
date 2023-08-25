/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AHeader.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 14:01:01 by treeps            #+#    #+#             */
/*   Updated: 2023/08/25 14:01:01 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_AHEADER_HPP
# define WEBSERV_AHEADER_HPP

# include <string>
# include <sstream>
# include <iostream>
# include <map>
# include "utils/utils.hpp"

class AHeader {
public:
	// constructors
	AHeader();
	AHeader(const std::string &rawHeader);
	AHeader(const AHeader &);

	// destructor
	~AHeader();

	// operator overload
	AHeader &operator=(const AHeader &);
	std::string &operator[](const std::string &key);

	// member functions
	virtual std::string exportHeader() = 0;

protected:
	// member variables
	std::string _rawHeader;
	std::map<std::string, std::string> _headerMap;
};

#endif
