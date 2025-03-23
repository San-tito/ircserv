/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cc                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:00:15 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/23 16:05:54 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"

Command::Command(std::string name, int min, int max) : name_(name), min_(min),
	max_(max)
{
}

Command::~Command(void)
{
}

bool Command::isUserRegistered(User *user)
{
	if (!user->registered())
	{
		// ERR_NOTREGISTERED;
		return (false);
	}
	return (true);
}

bool Command::isParamsValid(int size)
{
	if (size < min_ || (max_ != -1 && size > max_))
	{
		// ERR_NEEDMOREPARAMS;
		return (false);
	}
	return (true);
}

Command::Invite::Invite(void) : Command("INVITE", 2, 2)
{
}

void Command::Invite::execute(User *user,
	const std::vector<std::string> &params)
{
	if (!isUserRegistered(user))
		return ;
	if (!isParamsValid(params.size()))
		return ;
	// do stuff
}
