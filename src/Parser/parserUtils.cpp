/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parserUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrautne <mtrautne@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 23:06:50 by mtrautne          #+#    #+#             */
/*   Updated: 2023/11/05 23:10:28 by mtrautne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser/parserUtils.hpp"

void	checkBasicServerBlockIntegrity(const std::string &rawConfig) {
	if ((rawConfig.find("<server>", 0) == std::string::npos) ||
		rawConfig.find("</server>", 0) == std::string::npos)
		throw NoValidServerConfigException();
}

void 	checkServerBlockSeparators(const std::string &rawConfig,
									const size_t &start, const size_t &end) {
	if ((rawConfig.find("<server>", start) == std::string::npos) ||
		rawConfig.find("<server>", start) > end)
		throw ServerBlockSeparatorException();
	size_t	serverBlockStartDelimiter = rawConfig.find("<server>", start);
	if (rawConfig.find("<server>", serverBlockStartDelimiter + 9)
		< rawConfig.find("</server>", serverBlockStartDelimiter + 9))
		throw ServerBlockSeparatorException();
	size_t	serverBlockEndDelimiter = rawConfig.find("</server>", start);
	if (rawConfig.find("</server>", serverBlockEndDelimiter + 9)
		< rawConfig.find("<server>", serverBlockEndDelimiter + 9))
		throw ServerBlockSeparatorException();
}
