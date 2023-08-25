/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHeader.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 13:57:30 by treeps            #+#    #+#             */
/*   Updated: 2023/08/25 13:57:30 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_RESPONSEHEADER_HPP
# define WEBSERV_RESPONSEHEADER_HPP

# include <iostream>

# include <string>
# include <sstream>
# include <iostream>
# include <map>
# include "Header/AHeader.hpp"
# include "Response/responseCodes.hpp"
# include "utils/utils.hpp"

class ResponseHeader: public AHeader {
public:
	// constructors
	ResponseHeader();
	ResponseHeader(const std::string &rawHeader);
	ResponseHeader(const ResponseHeader &);

	// destructor
	~ResponseHeader();

	// operator overload
	ResponseHeader &operator=(const ResponseHeader &);

	// member functions
	std::string exportHeader();

private:
	// member functions
	void _parseHeader();
	void _parseLine(const std::string &line);
	void _parseFirstLine(std::string &line);
};

#endif
