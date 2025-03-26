#include "channel.h"
#include <algorithm>

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
	invites_[client->nickname()] = client;
}

bool Channel::IsInvited(Client *client) const
{
	return (invites_.find(client->nickname()) != invites_.end());
}

void Channel::AddMember(Client *client)
{
	members_[client->nickname()] = client;
}

void Channel::RemoveMember(Client *client)
{
	members_.erase(client->nickname());
	operators_.erase(client->nickname());
}

bool Channel::IsOperator(Client *client) const
{
	return (operators_.find(client->nickname()) != operators_.end());
}

void Channel::AddOperator(Client *client)
{
	operators_[client->nickname()] = client;
}

void Channel::RemoveOperator(Client *client)
{
	operators_.erase(client->nickname());
}

void Channel::Write(Client *sender, const std::string &message)
{
	std::map<std::string, Client *>::iterator it = members_.begin();
	for (; it != members_.end(); ++it)
	{
		if (it->second != sender)
			it->second->Write(sender->mask(), message);
	}
}

Client *Channel::IsMember(const std::string &name)
{
	std::map<std::string, Client *>::iterator it = members_.find(name);
	if (it != members_.end())
		return (it->second);
	return (0);
}

void Channel::Mode(Client *client, std::vector<std::string> &params)
// falta implementar muchas cosas
{
	char mode;

	if (params.size() <= 1)
	{
		for (std::set<char>::iterator it = modes_.begin(); it != modes_.end(); ++it)
			client->Write("MODE " + name() + " +" + *it);
		return ;
	}
	for (size_t i = 1; i < params.size(); i++)
	{
		mode = params[i][0];
		if (mode == '+')
		{
			AddMode(params[i][1]);
		}
		else if (mode == '-')
		{
			DelMode(params[i][1]);
		}
	}
}

void Channel::Join(Client *client)
{
	AddMember(client);
}

void Channel::Part(Client *client, const std::string &reason)
{
	std::map<std::string,
		Client *>::iterator it = members_.find(client->nickname());
	if (it != members_.end())
	{
		members_.erase(it);
		client->Write("PART " + name() + " :" + reason);
		Write(client, "PART " + name() + " :" + reason);
	}
}

void Channel::Kick(Client *client, Client *target, const std::string &reason)
{
	std::map<std::string,
		Client *>::iterator it = members_.find(target->nickname());
	if (it != members_.end())
	{
		members_.erase(it);
		Write(client, "KICK " + name() + " " + target->nickname() + " :"
			+ reason);
		target->Write("KICK " + name() + " :" + reason);
	}
}
