#include "channel.h"
#include "channelmanager.h"
#include "messages.h"
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
{
	Channel *channel;

	channel = Search(params[0]);
	if (!channel)
		return client->WritePrefix(ERR_NOSUCHCHANNEL(client->nickname(), params[0]));
	if(channel->name()[0] == '+')
	  return client->WritePrefix(ERR_NOCHANMODES(client->nickname(), params[0]));
	if (params.size() <= 1)
	 return client->WritePrefix(RPL_CHANNELMODEIS(client->nickname(),
				channel->name(), channel->modes()));
	if (!channel->IsOperator(client))
	  return client->WritePrefix(ERR_CHANOPRIVSNEEDED(client->nickname(),
				channel->name()));
  
	long l(0);
	std::string the_modes;
	std::string the_args;
	Client *target;

	for (size_t i = 1; i < params.size(); i++)
	{
		bool set(false);
		if(params[i][0] == '+')
		  set = true;
		else if (params[i][0] != '-')
		  return client->WritePrefix(ERR_NEEDMOREPARAMS(client->nickname(), "MODE"));
		switch (params[i][1])
		{
		  case 'o':
			if (i + 1 >= params.size())
			{
			  client->WritePrefix(ERR_NEEDMOREPARAMS(client->nickname(), "MODE"));
			  break;
			}
			target = Server::instance->clients().Search(params[i + 1]);
			if (!target)
			{
			  client->WritePrefix(ERR_NOSUCHNICK(client->nickname(), params[i + 1]));
			  break;
			}
			if (!channel->IsMember(target))
			{
			  client->WritePrefix(ERR_NOTONCHANNEL(client->nickname(), channel->name()));
			  break;
			}
			set ? channel->AddOperator(target) : channel->RemoveOperator(target);
			the_modes += (set ? std::string("+") : std::string("-")) + "o";
			the_args += " " + target->nickname();
			break;
		  case 'i':
			set ? channel->AddMode('i') : channel->DelMode('i');
			the_modes += (set ? std::string("+") : std::string("-")) + "i";
			break;
		  case 't':
			set ? channel->AddMode('t') : channel->DelMode('t');
			the_modes += (set ? std::string("+") : std::string("-")) + "t";
			break;
		  case 'k':
			if (!set)
			{
			  channel->DelMode('k');
			  the_modes += "-k";
			  break;
			}
			if (i + 1 >= params.size())
			{
			  client->WritePrefix(ERR_NEEDMOREPARAMS(client->nickname(), "MODE"));
			  break;
			}
			channel->DelMode('k');
			channel->set_key(params[i + 1]);
			channel->AddMode('k');
			the_modes += "+k";
			the_args += " " + params[i + 1];
			i++;
			break;
		  case 'l':
			if (!set)
			{
			  channel->DelMode('l');
			  the_modes += "-l";
			  break;
			}
			if (i + 1 >= params.size())
			{
			  client->WritePrefix(ERR_NEEDMOREPARAMS(client->nickname(), "MODE"));
			  break;
			}
			l = std::atol(params[i + 1].c_str());
			if (l <= 0 || l >= 0xFFFF)
			{
			  client->WritePrefix(ERR_INVALIDMODEPARAM(client->nickname(),
					  channel->name(), "l"));
			  break;
			}
			channel->DelMode('l');
			channel->set_max_members(l);
			channel->AddMode('l');
			the_modes += "+l";
			the_args += " " + params[i + 1];
			i++;
			break;
		  default:
			client->WritePrefix(ERR_UNKNOWNMODE(client->nickname(),
					channel->name(), params[i][1]));
			break;
		}
	}
	if(!the_modes.empty())
	{
	  client->Write(client->mask(), "MODE " + channel->name() + " " + the_modes + the_args);
	  channel->Write(client, "MODE " + channel->name() + " " + the_modes + the_args);
	}
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
		client->WritePrefix(ERR_NOSUCHCHANNEL(client->nickname(), channelName));
		return ;
	}
	client->Write(client->mask(), "PART " + channelName + " :" + reason);
	channel->Write(client, "PART " + channelName + " :" + reason);
	channel->RemoveMember(client);
}

void ChannelManager::PartAll(Client *client)
{
	std::map<std::string, Channel *>::iterator it = channels_.begin();
	for (; it != channels_.end(); ++it)
	  Part(client, it->first, "");
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
	if (!chan->IsMember(client))
		return (client->WritePrefix(ERR_NOTONCHANNEL(client->nickname(),
					channelName)));
	if (!chan->IsMember(target))
		return (client->WritePrefix(ERR_USENOTINCHANNEL(client->nickname(),
					target->nickname(), channelName)));
	if (!chan->IsOperator(client))
		return (client->WritePrefix(ERR_CHANOPPRIVTOOLOW(client->nickname(),
					channelName)));
	client->Write(client->mask(), "KICK " + channelName + " " + target->nickname() + " :" + reason);
	chan->Write(client, "KICK " + channelName + " " + target->nickname() + " :" + reason);
	chan->RemoveMember(target);
}
