/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cc                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:00:15 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/25 15:47:58 by ncastell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "server.h"

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

Invite::Invite(void) : Command("INVITE", 2, 2)
{
}

void Invite::Execute(User *user, const std::vector<std::string> &params)
{
	if (!UserRegistered(user))
		return ;
	if (!ParamsValid(user, params.size()))
		return ;
	User *target(Server::instance->users().Search(params[0]));
	if (!target)
	{
		// client->Error();
		return ;
	}
}
