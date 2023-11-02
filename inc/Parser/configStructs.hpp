/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigStructs.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrautne <mtrautne@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 11:00:27 by mtrautne          #+#    #+#             */
/*   Updated: 2023/11/02 11:01:54 by mtrautne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGSTRUCTS_HPP
# define CONFIGSTRUCTS_HPP

# include <string>
# include <map>

typedef struct s_locationConfig {
	std::string	path;
	std::string	root;
	std::string	index;
	std::string	cgiExtension;
	std::string	upload;
	std::string	redirect;
	std::string	allowedMethods;
	bool		autoIndex;
}	t_locationConfig;

typedef struct s_serverConfig {
	int										port;
	std::string								serverName;
	std::string								errorDir;
	std::map<std::string, t_locationConfig>	locationMap;
}	t_serverConfig;

# define RESET		"\033[0m"
# define GREEN		"\033[32m"
# define YELLOW		"\033[33m"
# define BLUE		"\033[34m"
# define RED		"\033[31m"
# define MAGENTA	"\033[35m"
# define CYAN		"\033[36m"
# define UP			"\033[A"
# define CUT		"\033[K"

#endif
