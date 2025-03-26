#include "channel.h"
#include "server.h"
#include "channelManager.h"
#include <algorithm>

ChannelManager::ChannelManager()
{
}

ChannelManager::~ChannelManager()
{

	std::vector<Channel *>::iterator it = channels_.begin();
	while (it != channels_.end())
	{
		delete *it;
		++it;
	}
}

bool ChannelManager::isValidName(const std::string &name)
{
	return (!name.empty() && (name[0] == '#' || name[0] == '&'
			|| name[0] == '+'));
}

Channel *ChannelManager::findChannel(const std::string &name)
{
	std::vector<Channel *>::iterator it = channels_.begin();
	while (it != channels_.end())
	{
		if ((*it)->getName() == name)
		{
			return (*it);
		}
		++it;
	}
	return (NULL);
}

bool ChannelManager::addChannel(Channel *channel)
{
	if (findChannel(channel->getName()))
	{
		return (false);
	}
	channels_.push_back(channel);
	return (true);
}

void ChannelManager::removeChannel(const std::string &name)
{

	std::vector<Channel *>::iterator it = channels_.begin();
	while (it != channels_.end())
	{
		if ((*it)->getName() == name)
		{
			channels_.erase(it);
			delete *it;
			return ;
		}
		++it;
	}
}

void ChannelManager::ExecuteMode(Client *client,
	std::vector<std::string> &params) // falta implementar muchas cosas
{
	Channel	*channel;

	channel = findChannel(params[0]);
	if (!channel)
	{
		// client->WriteErr(ERR_NOSUCHCHANNEL(client->getNick(), params[0]));
		return ;
	}
	channel->Mode(client, params);
}

void ChannelManager::ExecuteJoin(Client *client, const std::string &name)
{
	Channel	*channel;

	if (!isValidName(name))
	{
		// client->WriteErr(ERR_NOSUCHCHANNEL(client->getNick(), name));
		return ;
	}
	channel = findChannel(name);
	if (channel)
	{
	  if (channel->SearchUser(client->nickname()))
		return ;
	}
	else
	{
		channel = new Channel(name);
		addChannel(channel);
	}
	channel->Join(client);
}

void ChannelManager::ExecutePart(Client *client, const std::string &channelName,
	const std::string &reason)
{
	Channel	*channel;

	channel = findChannel(channelName);
	if (!channel)
	{
		// client->WriteErr(ERR_NOSUCHCHANNEL(client->getNick(), channelName));
		return ;
	}
	channel->Part(client, reason);
}

void ChannelManager::ExecutePartAll(Client *client)
{
	std::vector<Channel *>::iterator it = channels_.begin();
	while (it != channels_.end())
	{
		if (std::find((*it)->getClients().begin(), (*it)->getClients().end(),
				client) != (*it)->getClients().end())
		{
			(*it)->Part(client, "");
		}
		++it;
	}
}

void ChannelManager::ExecuteKick(Client *client, const std::string &nick,
	const std::string &channelName, const std::string &reason)
{
	Channel	*channel;

	channel = findChannel(channelName);
	Client *target(Server::instance->users().Search(nick));
	if (!target)
	{
	  return ;
	  //return (client->WriteErr(ERR_NOSUCHNICK(client->nickname(), nick));
	}
	if (!channel)
	{
		// client->WriteErr(ERR_NOSUCHCHANNEL(client->nickname(), channelName));
		return ;
	}
	if (!channel->SearchUser(client->nickname()))
	{
	  return;
		//return (client->WriteErr(ERR_NOTONCHANNEL(client->nickname(), channel)));
	}
	if (!channel->SearchUser(nick))
	{
	  return ;
		//return (client->WriteErr(ERR_USERNOTINCHANNEL(client->nickname(),
			//		target->getNick(), channel)));
	}
	if (!channel->isOperator(client))
	{
	  return ;
	  // return (client->WriteErr(ERR_CHANOPPRIVTOOLOW(client->nickname(),
		//			channel)));
	}
	channel->Kick(client, target, reason);
}
