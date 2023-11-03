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

#endif
