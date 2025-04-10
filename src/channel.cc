/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cc                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:19:06 by sguzman           #+#    #+#             */
/*   Updated: 2025/04/10 13:02:13 by bautrodr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.h"
#include "messages.h"
#include "server.h"
#include <sstream>

Channel::Channel(const std::string &name) : name_(name), max_members_(0)
{
}

Channel::~Channel(void)
{
}

std::string Channel::name(void) const
{
	return (name_);
}

std::string Channel::topic(void) const
{
	return (topic_);
}

std::string Channel::key(void) const
{
	return (key_);
}

void Channel::set_topic(std::string topic)
{
	topic_ = topic;
}

void Channel::set_key(std::string key)
{
	key_ = key;
}

void Channel::set_max_members(size_t max_members)
{
	max_members_ = max_members;
}

void Channel::AddMode(char mode)
{
	modes_.insert(mode);
}

bool Channel::HasMode(char mode) const
{
	return (modes_.find(mode) != modes_.end());
}

void Channel::DelMode(char mode)
{
	modes_.erase(mode);
}

void Channel::AddInvite(Client *client)
{
	invites_[client->socket()] = client;
}

bool Channel::IsMember(Client *client) const
{
	return (members_.find(client->socket()) != members_.end());
}

bool Channel::IsInvited(Client *client) const
{
	return (invites_.find(client->socket()) != invites_.end());
}

void Channel::AddMember(Client *client)
{
	members_[client->socket()] = client;
}

void Channel::RemoveMember(Client *client)
{
	members_.erase(client->socket());
	operators_.erase(client->socket());
	if (members_.empty())
		Server::instance->channels().RemoveChannel(this);
}

bool Channel::IsOperator(Client *client) const
{
	return (operators_.find(client->socket()) != operators_.end());
}

bool Channel::IsAllowedJoin(Client *client, const std::string &key)
{
	if (HasMode('i') && !IsInvited(client))
	{
		client->WritePrefix(ERR_INVITEONLYCHAN(client->nickname(), name_));
		return (false);
	}
	if (HasMode('k') && key_ != key)
	{
		client->WritePrefix(ERR_BADCHANNELKEY(client->nickname(), key));
		return (false);
	}
	if (HasMode('l') && max_members_ <= Channel::members_.size())
	{
		client->WritePrefix(ERR_CHANNELISFULL(client->nickname(), name_));
		return (false);
	}
	return (true);
}

void Channel::AddOperator(Client *client)
{
	operators_[client->socket()] = client;
}

void Channel::RemoveOperator(Client *client)
{
	operators_.erase(client->socket());
}

void Channel::Write(Client *sender, const std::string &message)
{
	std::map<int, Client *>::iterator it = members_.begin();
	if (!IsMember(sender))
		return (sender->WritePrefix(ERR_CANNOTSENDTOCHAN(sender->nickname(),
					name_)));
	for (; it != members_.end(); ++it)
	{
		if (it->second != sender)
			it->second->Write(sender->mask(), message);
	}
}

std::string Channel::modes(void) const
{
	std::string modes;
	std::string args;
	std::set<char>::iterator it = modes_.begin();
	for (; it != modes_.end(); ++it)
	{
		modes += *it;
		if (*it == 'k')
			args += " " + key_;
		else if (*it == 'l')
			args
				+= static_cast<std::ostringstream &>(std::ostringstream() << " " << max_members_).str();
	}
	return (modes + args);
}
