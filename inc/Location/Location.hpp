/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 15:43:00 by treeps            #+#    #+#             */
/*   Updated: 2023/09/08 15:43:00 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_LOCATION_HPP
# define WEBSERV_LOCATION_HPP

# include <string>
# include <iostream>

class Location {
public:
	// constructors
	Location();
	Location(const std::string& path,
			 const std::string& root,
			 const std::string& index,
			 const std::string& cgi,
			 const std::string& upload,
			 const std::string& tryFiles,
			 const std::string& redirect,
			 const std::string& allowedMethods,
			 bool autoindex);
	Location(const Location &);

	// destructor
	~Location();

	// operator overload
	Location &operator=(const Location &);

	// member functions

	// setter functions
	void setPath(const std::string &path);
	void setRoot(const std::string &root);
	void setIndex(const std::string &index);
	void setCgiExtension(const std::string &cgi);
	void setUpload(const std::string &upload);
	void setRedirect(const std::string &redirect);
	void setAllowedMethods(const std::string &allowedMethods);
	void setAutoindex(bool autoindex);

	// getter functions
	std::string getPath() const;
	std::string getRoot() const;
	std::string getIndex() const;
	std::string getCgiExtension() const;
	std::string getUpload() const;
	std::string getAlias() const;
	std::string getTryFiles() const;
	std::string getRedirect() const;
	std::string getAllowedMethods() const;
	bool getAutoindex() const;

private:
	// member variables
	std::string _path;
	std::string _root;
	std::string _alias;
	std::string _index;
	std::string _cgiExtension;
	std::string _upload;
	std::string _redirect;
	std::string _allowedMethods;
	std::string _tryFiles;
	bool _autoindex;
};

std::ostream& operator<<(std::ostream& output, const Location& object);

#endif
