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

class Location {
public:
	// constructors
	Location();
	Location(const std::string& path,
			 const std::string& root,
			 const std::string& index,
			 const std::string& cgi,
			 const std::string& upload,
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
	void setCgi(const std::string &cgi);
	void setUpload(const std::string &upload);
	void setAutoindex(bool autoindex);

	// getter functions
	std::string getPath() const;
	std::string getRoot() const;
	std::string getIndex() const;
	std::string getCgi() const;
	std::string getUpload() const;
	bool getAutoindex() const;

private:
	// member variables
	std::string _path;
	std::string _root;
	std::string _index;
	std::string _cgi;
	std::string _upload;
	bool _autoindex;
};

#endif
