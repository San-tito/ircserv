/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cc                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncastell <ncastell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:00:15 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/27 14:34:50 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "server.h"

Command::Command(std::string name, int min, int max,
	bool register_req) : name_(name), min_(min), max_(max),
	register_req_(register_req)
{
}

Command::~Command(void)
{
}

bool Command::ClientRegistered(Client *client)
{
	if (!client->registered() && register_req_)
	{
		client->WritePrefix(ERR_NOTREGISTERED(client->nickname()));
		return (false);
	}
	return (true);
}

bool Command::ParamsValid(Client *client, int size)
{
	if (size < min_ || (max_ != -1 && size > max_))
	{
		client->WritePrefix(ERR_NEEDMOREPARAMS(client->nickname(), name_));
		return (false);
	}
	return (true);
}

Invite::Invite(void) : Command("INVITE", 2, 2, true)
{
}

void Invite::Execute(Client *client, const std::vector<std::string> &params)
{
	static_cast<void>(client);
	Client *target(Server::instance->clients().Search(params[0]));
	if (!target)
	{
		// client.Error();
		return ;
	}
	/*
	Channel *chan(Server::instance->channels().Search(params[1]));
		if (chan)
		{
			if (!chan->IsMember(client))
			{
				client->WriteErr(ERR_NOTONCHANNEL(client->getNick(),
						params[1]));
				return ;
			}
			if (chan->HasMode('i') && !chan->IsOperator(client))
				{
					client->WriteErr(ERR_CHANOPRIVSNEEDED(client->getNick(),
							params[1]));
					return ;
				}
				else
					chan->AddInvite(target);
		}
		Log::Info() << "Client " << client->getNick() << " invites " << params[0] << " to " << params[1];
			target->Write("INVITE " + params[0] + " " + params[1]);
			target->WriteRpl(RPL_INVITING(target->getNick(), params[0],
					params[1]));
	*/
}

Join::Join(void) : Command("JOIN", 1, 2, true)
{
}

void Join::Execute(Client *client, const std::vector<std::string> &params)
{
	if (params.size() == 1 && params[0] == "0")
		return (Server::instance->channels().PartAll(client));
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
			if (chan->IsMember(client) || !JoinAllowed(client, chan, key))
				continue ;
		}
		else if (chan_name[0] != '+')
			op = true;
		if (!Server::instance->channels().Join(client, chan_name))
			continue ;
		if (!chan)
			chan = Server::instance->channels().Search(chan_name);
		if (op)
			chan->AddOperator(client);
		chan->Write(client, name_ + " :" + chan->name());
		client->Write(name_ + " :" + chan->getName());
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

Pass::Pass(void) : Command("PASS", 1, 2, false)
{
}

void Pass::Execute(Client *client, const std::vector<std::string> &params)
{
	if (client->password().empty())
	{
		Log() << "Connection " << client->socket() << ": got valid " << name_ << " command ...";
		client->set_password(params[0]);
	}
	else
		client->WritePrefix(ERR_ALREADYREGISTRED(client->nickname()));
}

Nick::Nick(void) : Command("NICK", 1, 2, false)
{
}

void Nick::Execute(Client *client, const std::vector<std::string> &params)
{
	if (client->nickname() != params[0])
	{
		if (!Server::instance->clients().IsValidNick(params[0]))
		{
			if (params[0].size() > MAX_NICK_LEN)
				return (client->WritePrefix(ERR_NICKNAMETOOLONG(client->nickname(),
							params[0])));
			else
				return (client->WritePrefix(ERR_ERRONEUSNICKNAME(client->nickname(),
							params[0])));
		}
		if (Server::instance->clients().Search(params[0]))
			return (client->WritePrefix(ERR_NICKNAMEINUSE(client->nickname(),
						params[0])));
	}
	if (!client->registered())
	{
		Log() << "Connection " << client->socket() << ": got valid " << name_ << " command ...";
		client->set_nickname(params[0]);
		if (!client->username().empty())
			return (client->Login());
	}
	else
	{
		Log() << "Connection " << client->socket() << ": changed nickname to " << params[0];
		client->set_nickname(params[0]);
		client->Write(client->mask(), name_ + " :" + params[0]);
	}
}

User::User(void) : Command("USER", 0, -1, false)
{
}

void User::Execute(Client *client, const std::vector<std::string> &params)
{
	if (client->registered())
		return (client->WritePrefix(ERR_ALREADYREGISTRED(client->nickname())));
	if (params.size() != 4)
		return (client->WritePrefix(ERR_NEEDMOREPARAMS(client->nickname(),
					name_)));
	if (!client->password().empty() || !client->nickname().empty())
	{
		if (params[0].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+-@._") != std::string::npos)
			return (Server::instance->clients().DelClient(client->socket()));
		Log() << "Connection " << client->socket() << ": got valid " << name_ << " command ...";
		client->set_username(params[0]);
		if (!client->nickname().empty())
			return (client->Login());
	}
	else
		client->WritePrefix(ERR_NOTREGISTERED(client->nickname()));
}
