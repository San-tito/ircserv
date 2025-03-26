#include "channel.h"
#include <algorithm>

Channel::Channel(const std::string &name) : name_(name), max_clients_(0),
	clients_(), invites_(), operators_()
{
}

Channel::~Channel()
{
}

std::string Channel::name() const
{
	return (name_);
}

std::set<char> Channel::modes() const
{
	return (modes_);
}

std::string Channel::topic() const
{
	return (topic_);
}

std::string Channel::key() const
{
	return (key_);
}

size_t Channel::maxClients() const
{
	return (max_clients_);
}

std::map<std::string, Client *> Channel::clients() const
{
	return (clients_);
}

void Channel::maxClients(size_t max_clients)
{
	max_clients_ = max_clients;
}

void Channel::topic(const std::string &topic)
{
	topic_ = topic;
}

void Channel::key(const std::string &key)
{
	key_ = key;
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

bool Channel::isInvited(Client *client) const
{
	return (invites_.find(client->nickname()) != invites_.end());
}

void Channel::AddClient(Client *client)
{
	clients_[client->nickname()] = client;
}

void Channel::RemoveClient(Client *client)
{
	clients_.erase(client->nickname());
	operators_.erase(client->nickname());
}

bool Channel::isOperator(Client *client) const
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
	for (std::map<std::string,
		Client *>::iterator it = clients_.begin(); it != clients_.end(); ++it)
	{
		if (it->second != sender)
			it->second->Write(sender->mask(), message);
	}
}

Client	*Channel::SearchClient(const std::string &name)
{
	std::map<std::string, Client *>::iterator it = clients_.find(name);
	if (it != clients_.end())
		return (it->second);
	return (NULL);
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
	AddClient(client);
}

void Channel::Part(Client *client, const std::string &reason)
{
	std::map<std::string, Client *>::iterator it = clients_.find(client->nickname());
	if (it != clients_.end())
	{
		clients_.erase(it);
		client->Write("PART " + name() + " :" + reason);
		Write(client, "PART " + name() + " :" + reason);
	}
}

void Channel::Kick(Client *client, Client *target, const std::string &reason)
{
	std::map<std::string,
		Client *>::iterator it = clients_.find(target->nickname());
	if (it != clients_.end())
	{
		clients_.erase(it);
		Write(client, "KICK " + name() + " " + target->nickname() + " :"
			+ reason);
		target->Write("KICK " + name() + " :" + reason);
	}
}
