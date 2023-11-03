/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserExceptions.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrautne <mtrautne@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 19:49:45 by mtrautne          #+#    #+#             */
/*   Updated: 2023/11/03 19:49:45 by mtrautne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PARSEREXCEPTIONS_HPP
# define PARSEREXCEPTIONS_HPP

# include <string>
# include <exception>
# include "utils/utils.hpp"

class InvalidArgNumException : public std::exception {
public:
	const char *what() const throw();
};

class CantOpenConfigException : public std::exception {
public:
	const char *what() const throw();
};

class EmptyConfigFileException: public std::exception {
public:
	const char *what() const throw();
};

class DuplicateServerNameException: public std::exception {
public:
	const char *what() const throw();
};

class InvalidGlobalValueException: public std::exception {
public:
	const char *what() const throw();
};
class InvalidConfigException: public std::exception {
public:
	const char *what() const throw();
};

class ValueMissingException : public std::exception {
public:
	virtual const char *what() const throw();
	ValueMissingException(const int &missingKey);
private:
	int	_key;
};

class MissingSemicolonException : public std::exception {
public:
	virtual const char* what() const throw();
};

class MissingClosingBracketException : public std::exception {
public:
	virtual const char* what() const throw();
};

class InvalidLocationException : public std::exception {
public:
	virtual const char* what() const throw();
};

class InvalidPathException : public std::exception {
public:
	virtual const char* what() const throw();
};

class InvalidPortException : public std::exception {
public:
	virtual const char* what() const throw();
};

class NoServerConfigException : public std::exception {
public:
	virtual const char* what() const throw();
};

class ExceededMaxServerNumberException : public std::exception {
public:
	virtual const char* what() const throw();
};

#endif
