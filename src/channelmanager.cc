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

void ChannelManager::ExecuteMode(User *user, std::vector<std::string> &params)
// falta implementar muchas cosas
{
	Channel *channel;

	channel = FindChannel(params[0]);
	if (!channel)
	{
		// user->WriteErr(ERR_NOSUCHCHANNEL(user->getNick(), params[0]));
		return ;
	}
	channel->Mode(user, params);
}

void ChannelManager::ExecuteJoin(User *user, const std::string &name)
{
	Channel	*channel;

	if (!IsValidName(name))
	{
		// user->WriteErr(ERR_NOSUCHCHANNEL(user->getNick(), name));
		return ;
	}
	channel = FindChannel(name);
	if (channel)
	{
		if (channel->SearchUser(user->nickname()))
			return ;
	}
	else
	{
		channel = new Channel(name);
		AddChannel(channel);
	}
	channel->Join(user);
}

void ChannelManager::ExecutePart(User *user, const std::string &channelName,
	const std::string &reason)
{
	Channel	*channel;

	channel = FindChannel(channelName);
	if (!channel)
	{
		// user->WriteErr(ERR_NOSUCHCHANNEL(user->getNick(), channelName));
		return ;
	}
	channel->Part(user, reason);
}

void ChannelManager::ExecutePartAll(User *user)
{
	for (std::map<std::string,
		Channel *>::iterator it = channels_.begin(); it != channels_.end(); ++it)
	{
		if (it->second->SearchUser(user->nickname()))
			it->second->Part(user, "");
	}
}

void ChannelManager::ExecuteKick(User *user, const std::string &nick,
	const std::string &channelName, const std::string &reason)
{
	Channel	*channel;

	channel = FindChannel(channelName);
	User *target(Server::instance->users().Search(nick));
	if (!target)
	{
		return ;
		// return (user->WriteErr(ERR_NOSUCHNICK(user->nickname(), nick));
	}
	if (!channel)
	{
		// user->WriteErr(ERR_NOSUCHCHANNEL(user->nickname(), channelName));
		return ;
	}
	if (!channel->SearchUser(user->nickname()))
	{
		return ;
		// return (user->WriteErr(ERR_NOTONCHANNEL(user->nickname(), channel)));
	}
	if (!channel->SearchUser(nick))
	{
		return ;
		// return (user->WriteErr(ERR_USERNOTINCHANNEL(user->nickname(),
		//		target->getNick(), channel)));
	}
	if (!channel->isOperator(user))
	{
		return ;
		// return (user->WriteErr(ERR_CHANOPPRIVTOOLOW(user->nickname(),
		//			channel)));
	}
	channel->Kick(user, target, reason);
}
