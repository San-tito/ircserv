/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cc                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncastell <ncastell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:00:15 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/26 15:17:55 by sguzman          ###   ########.fr       */
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
		user->WritePrefix(ERR_NOTREGISTERED(user->nickname()));
		return (false);
	}
	return (true);
}

bool Command::ParamsValid(User *user, int size)
{
	if (size < min_ || (max_ != -1 && size > max_))
	{
		user->WritePrefix(ERR_NEEDMOREPARAMS(user->nickname(), name_));
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
		// user.Error();
		return ;
	}
	/*
	Channel *chan(Server::instance->channels().Search(params[1]));
		if (chan)
		{
			if (!chan->IsMember(user))
			{
				client->WriteErr(ERR_NOTONCHANNEL(client->getNick(),
						params[1]));
				return ;
			}
			if (chan->HasMode('i') && !chan->IsOperator(user))
				{
					client->WriteErr(ERR_CHANOPRIVSNEEDED(client->getNick(),
							params[1]));
					return ;
				}
				else
					chan->AddInvite(target);
		}
		Log::Info() << "User " << client->getNick() << " invites " << params[0] << " to " << params[1];
			target->Write("INVITE " + params[0] + " " + params[1]);
			target->WriteRpl(RPL_INVITING(target->getNick(), params[0],
					params[1]));
	*/
}

Join::Join(void) : Command("JOIN", 1, 2)
{
}

void Join::Execute(User *user, const std::vector<std::string> &params)
{
	if (!UserRegistered(user))
		return ;
	if (!ParamsValid(user, params.size()))
		return ;
	if (params.size() == 1 && params[0] == "0")
	{
		// Server::instance->channels().PartAll(user);
		// return ;
	}
	// bool				op(false);
	std::string key;
	std::string chan_name;
	std::stringstream key_ss;
	std::stringstream chan_ss(params[0]);
	if (params.size() > 1)
	{
		key_ss.str(params[1]);
		key_ss.clear();
		std::getline(key_ss, key, ',');
	}
	while (std::getline(chan_ss, chan_name, ','))
	{
		/*
		Channel *chan(Server::instance->channels().Search(chan_name));
		if (chan)
		{
			if (chan->IsMember(user) || !JoinAllowed(user, chan, key))
				continue ;
		}
		else if (chan_name[0] != '+')
			op = true;
		if (!Server::instance->channels().Join(user, chan_name))
			continue ;
		if (!chan)
			chan = Server::instance->channels().Search(chan_name);
		if (op)
			chan->AddOperator(user);
		chan->Write(user, name_ + " :" + chan->name());
		user->Write(name_ + " :" + chan->getName());
		if (params.size() > 1)
			std::getline(key_ss, key, ',');
		*/
	}
	/*while (std::getline(chan_ss, channame, ','))
	{
		Channel *chan(Channel::Search(channame));
		Membership *is_member(chan ? Membership::Get(client, chan) : 0);
		if (is_member)
			continue ;
		if (chan)
		{
			if (!JoinAllowed(client, chan, key))
				continue ;
		}
		if (!chan && channame[0] != '+')
			op = true;
		if (!Channel::Join(client, channame))
			continue ;
		if (!chan)
		{
			chan = Channel::Search(channame);
			is_member = Membership::Get(client, chan);
		}
		if (op)
			is_member->AddMode('o');
		chan->Write(client, "JOIN :" + chan->getName());
		client->Write("JOIN :" + chan->getName());
		if (params.size() > 1)
			std::getline(key_ss, key, ',');
	}*/
}
