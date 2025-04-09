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
	Client *target(0);
	bool valid_mode(false);
	long limit_value(0);
	Channel *channel(Search(params[0]));
	if (!channel)
		return (client->WritePrefix(ERR_NOSUCHCHANNEL(client->nickname(),
					params[0])));
	if (channel->name()[0] == '+')
		return (client->WritePrefix(ERR_NOCHANMODES(client->nickname(),
					params[0])));
	if (params.size() <= 1)
		return (client->WritePrefix(RPL_CHANNELMODEIS(client->nickname(),
					channel->name(), channel->modes())));
	if (!channel->IsMember(client))
		return (client->WritePrefix(ERR_NOTONCHANNEL(client->nickname(),
					channel->name())));
	std::string the_modes;
	std::string the_args;
	bool is_adding(true);
	size_t mode_arg_index(1);
	size_t mode_param_index((params.size() > 2) ? 2 : static_cast<size_t>(-1));
	bool skip_first(false);
	const std::string &current_mode_str(params[mode_arg_index]);
	if (!current_mode_str.empty())
	{
		if (current_mode_str[0] == '+')
			is_adding = true;
		else if (current_mode_str[0] == '-')
			is_adding = false;
		else
		{
			is_adding = true;
			skip_first = true;
		}
	}
	the_modes = is_adding ? "+" : "-";
	while (mode_arg_index < params.size())
	{
		const std::string &mode_str(params[mode_arg_index]);
		std::string::size_type mode_pos(skip_first ? 0 : 1);
		skip_first = false;
		for (; mode_pos < mode_str.length(); ++mode_pos)
		{
			const char mode_char(mode_str[mode_pos]);
			if (mode_char == '+' || mode_char == '-')
			{
				bool new_adding((mode_char == '+'));
				if (new_adding != is_adding)
				{
					std::string::size_type last_pos(the_modes.length() - 1);
					if (the_modes[last_pos] == '+'
						|| the_modes[last_pos] == '-')
						the_modes[last_pos] = mode_char;
					else
						the_modes += mode_char;
					is_adding = new_adding;
				}
				continue ;
			}
			std::string param_value;
			target = 0;
			valid_mode = false;
			switch (mode_char)
			{
			case 'i':
			case 't':
				if (channel->IsOperator(client))
					valid_mode = true;
				else
					client->WritePrefix(ERR_CHANOPRIVSNEEDED(client->nickname(),
							channel->name()));
				break ;
			case 'k':
				if (!channel->IsOperator(client))
				{
					client->WritePrefix(ERR_CHANOPRIVSNEEDED(client->nickname(),
							channel->name()));
					break ;
				}
				if (!is_adding)
				{
					valid_mode = true;
					param_value = "*";
					if (mode_param_index != static_cast<size_t>(-1)
						&& mode_param_index < params.size())
						++mode_param_index;
				}
				else
				{
					if (mode_param_index == static_cast<size_t>(-1)
						|| mode_param_index >= params.size())
						return (client->WritePrefix(ERR_NEEDMOREPARAMS(client->nickname(),
									"MODE")));
					const std::string &key(params[mode_param_index]);
					if (key.empty() || key.find(' ') != std::string::npos)
						return (client->WritePrefix(ERR_INVALIDMODEPARAM(client->nickname(),
									channel->name(), 'k')));
					valid_mode = true;
					param_value = key;
					channel->set_key(key);
					++mode_param_index;
				}
				break ;
			case 'l':
				if (!channel->IsOperator(client))
				{
					client->WritePrefix(ERR_CHANOPRIVSNEEDED(client->nickname(),
							channel->name()));
					break ;
				}
				if (!is_adding)
					valid_mode = true;
				else
				{
					if (mode_param_index == static_cast<size_t>(-1)
						|| mode_param_index >= params.size())
						return (client->WritePrefix(ERR_NEEDMOREPARAMS(client->nickname(),
									"MODE")));
					const std::string &limit_str(params[mode_param_index]);
					limit_value = std::atol(limit_str.c_str());
					if (limit_value <= 0 || limit_value >= 0xFFFF)
						return (client->WritePrefix(ERR_INVALIDMODEPARAM(client->nickname(),
									channel->name(), 'l')));
					valid_mode = true;
					param_value = limit_str;
					channel->set_max_members(limit_value);
					++mode_param_index;
				}
				break ;
			case 'o':
				if (!channel->IsOperator(client))
				{
					client->WritePrefix(ERR_CHANOPRIVSNEEDED(client->nickname(),
							channel->name()));
					break ;
				}
				if (mode_param_index != static_cast<size_t>(-1)
					&& mode_param_index < params.size())
				{
					const std::string &target_nick(params[mode_param_index]);
					target = Server::instance->clients().Search(target_nick);
					if (!target)
						client->WritePrefix(ERR_NOSUCHNICK(client->nickname(),
								target_nick));
					else if (!channel->IsMember(target))
						client->WritePrefix(ERR_USENOTINCHANNEL(client->nickname(),
								target_nick, channel->name()));
					else
					{
						valid_mode = true;
						param_value = target->nickname();
					}
					++mode_param_index;
				}
				else
					return (client->WritePrefix(ERR_NEEDMOREPARAMS(client->nickname(),
								"MODE")));
				break ;
			default:
				client->WritePrefix(ERR_UNKNOWNMODE(client->nickname(),
						channel->name(), std::string(1, mode_char)));
				break ;
			}
			if (valid_mode)
			{
				if (mode_char == 'o' && target)
				{
					if (is_adding)
						channel->AddOperator(target);
					else
						channel->RemoveOperator(target);
				}
				else
				{
					if (is_adding)
						channel->AddMode(mode_char);
					else
						channel->DelMode(mode_char);
				}
				the_modes += mode_char;
				if (!param_value.empty())
					the_args += " " + param_value;
			}
		}
		if (mode_param_index > mode_arg_index)
			mode_arg_index = mode_param_index;
		else
			++mode_arg_index;
		if (mode_arg_index < params.size() && mode_param_index < params.size())
			mode_param_index = mode_arg_index + 1;
		else
			mode_param_index = static_cast<size_t>(-1);
		skip_first = true;
	}
	if (!the_modes.empty())
	{
		std::string::size_type last_pos(the_modes.length() - 1);
		if (the_modes[last_pos] == '+' || the_modes[last_pos] == '-')
			the_modes.resize(last_pos);
	}
	if (the_modes.length() > 1)
	{
		std::string message("MODE " + channel->name() + " " + the_modes
			+ the_args);
		client->Write(client->mask(), message);
		channel->Write(client, message);
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
	client->Write(client->mask(), "KICK " + channelName + " "
		+ target->nickname() + " :" + reason);
	chan->Write(client, "KICK " + channelName + " " + target->nickname() + " :"
		+ reason);
	chan->RemoveMember(target);
}
