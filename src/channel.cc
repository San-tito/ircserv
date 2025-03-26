#include "channel.h"
#include <algorithm>

Channel::Channel(const std::string &name) : name_(name), max_users_(0),
	users_(), invites_(), operators_()
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

size_t Channel::maxUsers() const
{
	return (max_users_);
}

std::map<std::string, User *> Channel::users() const
{
	return (users_);
}

void Channel::maxUsers(size_t max_users)
{
	max_users_ = max_users;
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

void Channel::AddInvite(User *user)
{
	invites_[user->nickname()] = user;
}

bool Channel::isInvited(User *user) const
{
	return (invites_.find(user->nickname()) != invites_.end());
}

void Channel::AddUser(User *user)
{
	users_[user->nickname()] = user;
}

void Channel::RemoveUser(User *user)
{
	users_.erase(user->nickname());
	operators_.erase(user->nickname());
}

bool Channel::isOperator(User *user) const
{
	return (operators_.find(user->nickname()) != operators_.end());
}

void Channel::AddOperator(User *user)
{
	operators_[user->nickname()] = user;
}

void Channel::RemoveOperator(User *user)
{
	operators_.erase(user->nickname());
}

void Channel::Write(User *sender, const std::string &message)
{
	for (std::map<std::string,
		User *>::iterator it = users_.begin(); it != users_.end(); ++it)
	{
		if (it->second != sender)
			it->second->Write(sender->mask(), message);
	}
}

User *Channel::SearchUser(const std::string &name)
{
	std::map<std::string, User *>::iterator it = users_.find(name);
	if (it != users_.end())
		return (it->second);
	return (NULL);
}

void Channel::Mode(User *user, std::vector<std::string> &params)
// falta implementar muchas cosas
{
	char mode;

	if (params.size() <= 1)
	{
		for (std::set<char>::iterator it = modes_.begin(); it != modes_.end(); ++it)
			user->Write("MODE " + name() + " +" + *it);
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

void Channel::Join(User *user)
{
	AddUser(user);
}

void Channel::Part(User *user, const std::string &reason)
{
	std::map<std::string, User *>::iterator it = users_.find(user->nickname());
	if (it != users_.end())
	{
		users_.erase(it);
		user->Write("PART " + name() + " :" + reason);
		Write(user, "PART " + name() + " :" + reason);
	}
}

void Channel::Kick(User *user, User *target, const std::string &reason)
{
	std::map<std::string,
		User *>::iterator it = users_.find(target->nickname());
	if (it != users_.end())
	{
		users_.erase(it);
		Write(user, "KICK " + name() + " " + target->nickname() + " :"
			+ reason);
		target->Write("KICK " + name() + " :" + reason);
	}
}
