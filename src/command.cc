/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cc                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncastell <ncastell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:00:15 by sguzman           #+#    #+#             */
/*   Updated: 2025/04/01 13:03:22 by sguzman          ###   ########.fr       */
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
	Client *target(Server::instance->clients().Search(params[0]));
	if (!target)
		return (client->WritePrefix(ERR_NOSUCHNICK(client->nickname(),
					params[0])));
	Channel *chan(Server::instance->channels().Search(params[1]));
	if (chan)
	{
		if (!chan->IsMember(client))
			return (client->WritePrefix(ERR_NOTONCHANNEL(client->nickname(),
						params[1])));
		if (chan->HasMode('i') && !chan->IsOperator(client))
			return (client->WritePrefix(ERR_CHANOPRIVSNEEDED(client->nickname(),
						params[1])));
		else
			chan->AddInvite(target);
	}
	Log() << "Client " << client->nickname() << " invites " << params[0] << " to " << params[1];
	target->Write("INVITE " + params[0] + " " + params[1]);
	target->WritePrefix(RPL_INVITING(target->nickname(), params[0], params[1]));
}

Join::Join(void) : Command("JOIN", 1, 2, true)
{
}

void Join::Execute(Client *client, const std::vector<std::string> &params)
{
	if (params.size() == 1 && params[0] == "0")
		return (Server::instance->channels().PartAll(client));
	bool op(false);
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
		Channel *chan(Server::instance->channels().Search(chan_name));
		if (chan)
		{
			if (chan->IsMember(client) || !chan->IsAllowedJoin(client, key))
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
		client->Write(name_ + " :" + chan->name());
		if (params.size() > 1)
			std::getline(key_ss, key, ',');
	}
	while (std::getline(chan_ss, chan_name, ','))
	{
		Channel *chan(Server::instance->channels().Search(chan_name));
		if (chan->IsMember(client))
			continue ;
		if (chan && !chan->IsAllowedJoin(client, key))
			continue ;
		if (!chan && chan_name[0] != '+')
			op = true;
		if (!Server::instance->channels().Join(client, chan_name))
			continue ;
		if (!chan)
			chan = Server::instance->channels().Search(chan_name);
		if (op)
			chan->AddOperator(client);
		chan->Write(client, name_ + " :" + chan->name());
		client->Write(name_ + " :" + chan->name());
		if (params.size() > 1)
			std::getline(key_ss, key, ',');
	}
}

Pass::Pass(void) : Command("PASS", 0, -1, false)
{
}

void Pass::Execute(Client *client, const std::vector<std::string> &params)
{
	if (client->registered())
		return (client->WritePrefix(ERR_ALREADYREGISTRED(client->nickname())));
	if (params.size() != 1)
		return (client->WritePrefix(ERR_NEEDMOREPARAMS(client->nickname(),
					name_)));
	if (client->password().empty())
	{
		Log() << "Connection " << client->socket() << ": got valid " << name_ << " command ...";
		client->set_password(params[0]);
	}
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
			return (Server::instance->clients().CloseClient(client->socket(),
					"Invalid username"));
		Log() << "Connection " << client->socket() << ": got valid " << name_ << " command ...";
		client->set_username(params[0]);
		if (!client->nickname().empty())
			return (client->Login());
	}
	else
		client->WritePrefix(ERR_NOTREGISTERED(client->nickname()));
}

PrivMsg::PrivMsg(void) : Command("PRIVMSG", 0, 2, true)
{
}

void PrivMsg::Execute(Client *client, const std::vector<std::string> &params)
{
	if (params.size() == 0)
		return (client->WritePrefix(ERR_NORECIPIENT(client->nickname(),
					name_)));
	else if (params.size() == 1)
		return (client->WritePrefix(ERR_NOTEXTTOSEND(client->nickname())));
	std::string target;
	std::stringstream ss(params[0]);
	while (std::getline(ss, target, ','))
	{
		Client *dest(0);
		Channel *chan(0);
		if (target.find('!') != std::string::npos)
			dest = Server::instance->clients().Search(target);
		else
		{
			std::string nick(target.substr(0, target.find('!')));
			dest = Server::instance->clients().Search(nick);
		}
		if (dest)
			dest->Write(client->mask(), name_ + " " + client->nickname() + " :"
				+ params[1]);
		else if ((chan = Server::instance->channels().Search(target)))
			chan->Write(client, name_ + " " + client->nickname() + " :"
				+ params[1]);
		else
			client->WritePrefix(ERR_NOSUCHNICK(client->nickname(), target));
	}
}

Part::Part(void) : Command("PART", 1, 2, true)
{
}

void Part::Execute(Client *client, const std::vector<std::string> &params)
{
	if (params[0].empty())
		return (client->WritePrefix(ERR_NEEDMOREPARAMS(client->nickname(),
					name_)));
	std::string chan;
	std::stringstream ss(params[0]);
	while (std::getline(ss, chan, ','))
		Server::instance->channels().Part(client, params[0],
			params.size() > 1 ? params[1] : "");
}

Kick::Kick(void) : Command("KICK", 2, 3, true)
{
}

void Kick::Execute(Client *client, const std::vector<std::string> &params)
{
	unsigned int channel_count(0);
	unsigned int nick_count(0);
	std::string item;
	std::stringstream ss(params[0]);
	while (std::getline(ss, item, ','))
		channel_count++;
	ss.str(params[1]);
	ss.clear();
	while (std::getline(ss, item, ','))
		nick_count++;
	std::string current_nick;
	std::string current_channel;
	std::string reason(params.size() == 3 ? params[2] : client->nickname());
	if (channel_count == 1)
	{
		ss.str(params[1]);
		ss.clear();
		while (std::getline(ss, current_nick, ','))
		{
			Server::instance->channels().Kick(client, current_nick, params[0],
				reason);
			nick_count--;
		}
	}
	else if (channel_count == nick_count)
	{
		ss.str(params[0]);
		ss.clear();
		std::stringstream nick_ss(params[1]);
		nick_ss.clear();
		while (std::getline(ss, current_channel, ',') && std::getline(nick_ss,
				current_nick, ','))
		{
			Server::instance->channels().Kick(client, current_nick,
				current_channel, reason);
			channel_count--;
			nick_count--;
		}
	}
	else
		client->WritePrefix(ERR_NEEDMOREPARAMS(client->nickname(), name_));
}

Quit::Quit(void) : Command("QUIT", 0, 1, true)
{
}

void Quit::Execute(Client *client, const std::vector<std::string> &params)
{
	if (params.size() == 1)
		Log() << client->nickname() << ": " << params[0];
	Server::instance->clients().CloseClient(client->socket(), "Quit: "
		+ (params.size() == 1 ? params[0] : ""));
}

Mode::Mode(void) : Command("MODE", 1, -1, true)
{
}

void Mode::Execute(Client *client, const std::vector<std::string> &params)
{
	Channel *chan(0);
	bool is_valid_nick(Server::instance->clients().IsValidNick(params[0]));
	bool is_valid_chan(Server::instance->channels().IsValidName(params[0]));
	if (is_valid_chan)
		chan = Server::instance->channels().Search(params[0]);
	if (chan)
		return (Server::instance->channels().Mode(client, params));
	if (is_valid_nick)
		client->WritePrefix(ERR_NOSUCHNICK(client->nickname(), params[0]));
	else
		client->WritePrefix(ERR_NOSUCHCHANNEL(client->nickname(), params[0]));
}

Topic::Topic(void) : Command("TOPIC", 1, 2, true)
{
}

void Topic::Execute(Client *client, const std::vector<std::string> &params)
{
	Channel *chan(Server::instance->channels().Search(params[0]));
	if (!chan)
		return (client->WritePrefix(ERR_NOSUCHCHANNEL(client->nickname(),
					params[0])));
	if (!chan->IsMember(client))
		return (client->WritePrefix(ERR_NOTONCHANNEL(client->nickname(),
					params[0])));
	if (params.size() == 1)
	{
		std::string topic(chan->topic());
		if (topic.empty())
			client->WritePrefix(RPL_NOTOPIC(chan->name(), client->nickname()));
		else
			client->WritePrefix(RPL_TOPIC(client->nickname(), chan->name(),
					topic));
		return ;
	}
	if (chan->HasMode('t') && !chan->IsOperator(client))
		return (client->WritePrefix(ERR_CHANOPRIVSNEEDED(client->nickname(),
					params[0])));
	if (params[1] != chan->topic())
		chan->Write(client, name_ + " " + params[0] + " :" + params[1]);
	chan->set_topic(params[1]);
	client->Write(name_ + " " + params[0] + " :" + params[1]);
}
