/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 15:38:10 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/23 16:06:29 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

CommandParser::CommandParser(void)
{
	commands_["INVITE"] = new Command::Invite();
}

CommandParser::~CommandParser(void)
{
	std::map<std::string, Command *>::iterator it(commands_.begin());
	for (; it != commands_.end(); ++it)
		delete it->second;
}

void CommandParser::ProcessCommand(User *user, std::string const &command)
{
	(void)user;
	(void)command;
}
