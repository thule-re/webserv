/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parserUtils.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrautne <mtrautne@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 23:07:03 by mtrautne          #+#    #+#             */
/*   Updated: 2023/11/05 23:07:39 by mtrautne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSERUTILS_HPP
# define PARSERUTILS_HPP

# include "Parser/ParserExceptions.hpp"

void	checkBasicServerBlockIntegrity(const std::string &rawConfig);
void 	checkServerBlockSeparators(const std::string &rawConfig,
								const size_t &start, const size_t &end);
#endif
