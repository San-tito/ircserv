#include "channel.h"
#include <algorithm>

Channel::Channel(const std::string &name) : name_(name), max_users_(0), clients_(0), invites_(0), operators_()
{
}

Channel::~Channel()
{
}

std::string Channel::getName() const
{
	return (name_);
}

std::set<char> Channel::getModes() const
{
	return (modes_);
}

std::string Channel::getTopic() const
{
	return (topic_);
}

std::string Channel::getKey() const
{
	return (key_);
}

size_t Channel::getMaxUsers() const
{
	return (max_users_);
}

std::vector<Client *> Channel::getClients() const
{
	return (clients_);
}

void Channel::setMaxUsers(size_t max_users)
{
	max_users_ = max_users;
}

void Channel::setTopic(const std::string &topic)
{
	topic_ = topic;
}

void Channel::setKey(const std::string &key)
{
	key_ = key;
}

void Channel::addMode(char mode)
{
	modes_.insert(mode);
}

bool Channel::hasMode(char mode) const
{
	return (modes_.find(mode) != modes_.end());
}

void Channel::delMode(char mode)
{
	modes_.erase(mode);
}

void Channel::addInvite(Client *client)
{
	invites_.push_back(client);
}

bool Channel::isInvited(Client *client) const
{
	for (std::vector<Client *>::const_iterator it = invites_.begin(); it != invites_.end(); ++it)
	{
		if (*it == client)
			return (true);
	}
	return (false);
}

void Channel::addClient(Client *client)
{
	clients_.push_back(client);
}

void Channel::removeClient(Client *client)
{
	for (std::vector<Client *>::iterator it = clients_.begin(); it != clients_.end(); ++it)
	{
		if (*it == client)
		{
			clients_.erase(it);
			return ;
		}
	}
	operators_.erase(client);
}

bool Channel::isOperator(Client *client) const
{
	return (operators_.find(client) != operators_.end());
}

void Channel::addOperator(Client *client)
{
	operators_.insert(client);
}

void Channel::removeOperator(Client *client)
{
	operators_.erase(client);
}

void Channel::Write(Client *sender, const std::string &message)
{
	for (std::vector<Client *>::iterator it = clients_.begin(); it != clients_.end(); ++it)
	{
		if (*it != sender)
			(*it)->Write(sender->prefix() + " " + message);
	}
}

Client *Channel::SearchUser(const std::string &name)
{
	for (std::vector<Client *>::iterator it = clients_.begin(); it != clients_.end(); ++it)
	{
		if ((*it)->nickname() == name)
			return (*it);
	}
	return (NULL);
}

void Channel::Mode(Client *client, std::vector<std::string> &params) // falta implementar muchas cosas
{
	char	mode;

	if (params.size() <= 1)
	{
		for (std::set<char>::iterator it = modes_.begin(); it != modes_.end(); ++it)
			client->Write("MODE " + getName() + " +" + *it);
		return ;
	}
	for (size_t i = 1; i < params.size(); i++)
	{
		mode = params[i][0];
		if (mode == '+')
		{
			addMode(params[i][1]);
		}
		else if (mode == '-')
		{
			delMode(params[i][1]);
		}
	}
}

void Channel::Join(Client *client)
{
	clients_.push_back(client);
}

void Channel::Part(Client *client, const std::string &reason)
{
	std::vector<Client *>::iterator it = std::find(clients_.begin(),
			clients_.end(), client);
	if (it != clients_.end())
	{
		clients_.erase(it);
		client->Write("PART " + getName() + " :" + reason);
		Write(client, "PART " + getName() + " :" + reason);
	}
}

void Channel::Kick(Client *client, Client *target, const std::string &reason)
{
	std::vector<Client *>::iterator it = std::find(clients_.begin(),
			clients_.end(), target);
	if (it != clients_.end())
	{
		clients_.erase(it);
		Write(client, "KICK " + getName() + " " + target->nickname() + " :"
			+ reason);
		target->Write("KICK " + getName() + " :" + reason);
	}
}
