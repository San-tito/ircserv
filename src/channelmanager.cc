#include "channel.h"
#include "channelmanager.h"
#include "server.h"

ChannelManager::ChannelManager(void)
{
}

ChannelManager::~ChannelManager(void)
{
	std::map<std::string, Channel *>::iterator it = channels_.begin();
	for (; it != channels_.end(); ++it)
		delete it->second;
}

bool ChannelManager::IsValidName(const std::string &name)
{
	return (!name.empty() && (name[0] == '#' || name[0] == '&'
			|| name[0] == '+'));
}

Channel *ChannelManager::Search(const std::string &name)
{
	std::map<std::string, Channel *>::iterator it = channels_.find(name);
	if (it != channels_.end())
		return (it->second);
	return (0);
}

bool ChannelManager::AddChannel(Channel *channel)
{
	if (Search(channel->name()))
		return (false);
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

void ChannelManager::Mode(Client *client,
	const std::vector<std::string> &params)
// falta implementar muchas cosas
{
	Channel *channel;

	channel = Search(params[0]);
	if (!channel)
	{
		// client->WriteErr(ERR_NOSUCHCHANNEL(client->getNick(), params[0]));
		return ;
	}
	channel->Mode(client, params);
}

bool ChannelManager::Join(Client *client, const std::string &name)
{
	Channel	*channel;

	if (!IsValidName(name))
	{
		client->WritePrefix(ERR_NOSUCHCHANNEL(client->nickname(), name));
		return (false);
	}
	channel = Search(name);
	if (channel)
	{
		if (channel->IsMember(client))
			return (false);
	}
	else
	{
		channel = new Channel(name);
		AddChannel(channel);
	}
	channel->AddMember(client);
	return (true);
}

void ChannelManager::Part(Client *client, const std::string &channelName,
	const std::string &reason)
{
	Channel	*channel;

	channel = Search(channelName);
	if (!channel)
	{
		// client->WriteErr(ERR_NOSUCHCHANNEL(client->getNick(), channelName));
		return ;
	}
	channel->Part(client, reason);
}

void ChannelManager::PartAll(Client *client)
{
	std::map<std::string, Channel *>::iterator it = channels_.begin();
	for (; it != channels_.end(); ++it)
	{
		if (it->second->IsMember(client))
			it->second->Part(client, "");
	}
}

void ChannelManager::Kick(Client *client, const std::string &nick,
	const std::string &channelName, const std::string &reason)
{
	Client *target(Server::instance->clients().Search(nick));
	if (!target)
		return (client->WritePrefix(ERR_NOSUCHNICK(client->nickname(), nick)));
	Channel *chan(Server::instance->channels().Search(channelName));
	if (!chan)
		return (client->WritePrefix(ERR_NOSUCHCHANNEL(client->nickname(),
					channelName)));
	if (chan->IsMember(client))
		return (client->WritePrefix(ERR_NOTONCHANNEL(client->nickname(),
					channelName)));
	if (chan->IsMember(target))
		return (client->WritePrefix(ERR_USENOTINCHANNEL(client->nickname(),
					target->nickname(), channelName)));
	if (!chan->IsOperator(client))
		return (client->WritePrefix(ERR_CHANOPPRIVTOOLOW(client->nickname(),
					channelName)));
	chan->RemoveMember(target);
	client->Write("KICK " + channelName + " " + nick + " :" + reason);
	chan->Write(client, "KICK " + channelName + " " + nick + " :" + reason);
	// Log::Info() << "User " << nick << " was kicked from channel " << channel << " by " << client->getNick() << " (" << reason << ")\n";
}
