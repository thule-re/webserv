/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrautne <mtrautne@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 10:31:42 by mtrautne          #+#    #+#             */
/*   Updated: 2023/08/23 10:31:42 by mtrautne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <string>
# include <iostream>
# include <sstream>
# include <fstream>
# include <map>
# include <vector>
# include <exception>
#include "Parser/errorCodes.hpp"

class Config {
public:
	// constructors
	Config(std::string &rawConfig);
	Config();

	Config(const Config& other);

	// destructor
	~Config();

	// operator overload
	Config &operator=(const Config &other);

	// exceptions
	class ValueMissingException : public std::exception {
	public:
		virtual const char *what() const _NOEXCEPT;
		ValueMissingException(const int &missingKey);
	private:
		int	_key;
	};
	class NotADirectoryException : public std::exception {
	public:
		virtual const char* what() const _NOEXCEPT;
	};
	class MissingSemicolonException : public std::exception {
	public:
		virtual const char* what() const _NOEXCEPT;
	};
	class NoValidMethodException : public std::exception {
	public:
		virtual const char* what() const _NOEXCEPT;
	};
	class InvalidHtmlException : public std::exception {
	public:
		virtual const char* what() const _NOEXCEPT;
	};
	class InvalidPortException : public std::exception {
	public:
		virtual const char* what() const _NOEXCEPT;
	};
	class EmptyValueException : public std::exception {
	public:
		virtual const char* what() const _NOEXCEPT;
	};

	// member functions
	std::map<std::string, std::string>	getMap() const;

private:
	std::map<std::string, std::string>	_configMap;
	void	parseConfig(const std::string &configBlock);
	void	populateConfig(const std::string &configBlock);
	void 	setGlobalValues(const std:: string &configBlock);
	void	setValue(const int key, const std::string &configBlock);
	void	setLocations(const std::string &configBlock);
	void 	splitLocationBlocks(std::vector<std::string> &, const std::string &);
	void 	validateNoEmptyEntry();
	void	validateConfigDirs();
	void	validateDir(std::string const &directory);
	void 	validateMethods();
	void	validateHtml();
	void	validatePort();
};

#endif
