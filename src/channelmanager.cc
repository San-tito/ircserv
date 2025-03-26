#include "channel.h"
#include "channelmanager.h"
#include "server.h"
#include <algorithm>

ChannelManager::ChannelManager()
{
}

ChannelManager::~ChannelManager()
{
	for (std::map<std::string,
		Channel *>::iterator it = channels_.begin(); it != channels_.end(); ++it)
		delete it->second;
}

bool ChannelManager::IsValidName(const std::string &name)
{
	return (!name.empty() && (name[0] == '#' || name[0] == '&'
			|| name[0] == '+'));
}

Channel *ChannelManager::FindChannel(const std::string &name)
{
	std::map<std::string, Channel *>::iterator it = channels_.find(name);
	if (it != channels_.end())
	{
		return (it->second);
	}
	return (NULL);
}

bool ChannelManager::AddChannel(Channel *channel)
{
	if (FindChannel(channel->name()))
	{
		return (false);
	}
	channels_[channel->name()] = channel;
	return (true);
}

void ChannelManager::RemoveChannel(const std::string &name)
{
	std::map<std::string, Channel *>::iterator it = channels_.find(name);
	if (it != channels_.end())
	{
		delete it->second;
		channels_.erase(it);
	}
}

void ChannelManager::ExecuteMode(Client *client, std::vector<std::string> &params)
// falta implementar muchas cosas
{
	Channel *channel;

	channel = FindChannel(params[0]);
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

	if (!IsValidName(name))
	{
		// client->WriteErr(ERR_NOSUCHCHANNEL(client->getNick(), name));
		return ;
	}
	channel = FindChannel(name);
	if (channel)
	{
		if (channel->SearchClient(client->nickname()))
			return ;
	}
	else
	{
		channel = new Channel(name);
		AddChannel(channel);
	}
	channel->Join(client);
}

void ChannelManager::ExecutePart(Client *client, const std::string &channelName,
	const std::string &reason)
{
	Channel	*channel;

	channel = FindChannel(channelName);
	if (!channel)
	{
		// client->WriteErr(ERR_NOSUCHCHANNEL(client->getNick(), channelName));
		return ;
	}
	channel->Part(client, reason);
}

void ChannelManager::ExecutePartAll(Client *client)
{
	for (std::map<std::string,
		Channel *>::iterator it = channels_.begin(); it != channels_.end(); ++it)
	{
		if (it->second->SearchClient(client->nickname()))
			it->second->Part(client, "");
	}
}

void ChannelManager::ExecuteKick(Client *client, const std::string &nick,
	const std::string &channelName, const std::string &reason)
{
	Channel	*channel;

	channel = FindChannel(channelName);
	Client *target(Server::instance->clients().Search(nick));
	if (!target)
	{
		return ;
		// return (client->WriteErr(ERR_NOSUCHNICK(client->nickname(), nick));
	}
	if (!channel)
	{
		// client->WriteErr(ERR_NOSUCHCHANNEL(client->nickname(), channelName));
		return ;
	}
	if (!channel->SearchClient(client->nickname()))
	{
		return ;
		// return (client->WriteErr(ERR_NOTONCHANNEL(client->nickname(), channel)));
	}
	if (!channel->SearchClient(nick))
	{
		return ;
		// return (client->WriteErr(ERR_CLIENTNOTINCHANNEL(client->nickname(),
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
