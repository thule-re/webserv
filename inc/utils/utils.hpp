/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 18:21:59 by treeps            #+#    #+#             */
/*   Updated: 2023/08/21 18:21:59 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_UTILS_HPP
# define WEBSERV_UTILS_HPP

# include <fstream>
# include <string>
# include <sstream>

# define HTTP_VERSION "HTTP/1.1"
# define METHOD_GET "GET"
# define METHOD_POST "POST"
# define METHOD_DELETE "DELETE"
# define CRLF "\r\n"

// function prototypes
void replaceAll(std::string &str, const std::string &oldStr, const std::string &newStr);

std::string readFile(std::ifstream &file);
std::string readFile(const std::string &path);
std::string getContentType(const std::string& path);

// template functions
template<typename T> std::string toString(const T& value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

# include "utils.tpp"

#endif
