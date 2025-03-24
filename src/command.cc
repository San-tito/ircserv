/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cc                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:00:15 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/24 21:10:16 by sguzman          ###   ########.fr       */
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

bool Command::UserRegistered(User *user)
{
	if (!user->registered())
	{
		user->WriteErrNotRegistered();
		return (false);
	}
	return (true);
}

bool Command::ParamsValid(User *user, int size)
{
	if (size < min_ || (max_ != -1 && size > max_))
	{
		user->WriteErrNeedMoreParams(name_);
		return (false);
	}
	return (true);
}

Command::Invite::Invite(void) : Command("INVITE", 2, 2)
{
}

void Command::Invite::Execute(User *user,
	const std::vector<std::string> &params)
{
	if (!UserRegistered(user))
		return ;
	if (!ParamsValid(user, params.size()))
		return ;
	// do stuff
}
