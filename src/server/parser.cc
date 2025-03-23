/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 15:38:10 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/23 15:38:40 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

CommandParser::CommandParser(void)
{
}

CommandParser::~CommandParser(void)
{
}

void CommandParser::ProcessCommand(User *user, std::string const &command)
{
	(void)user;
	(void)command;
}
