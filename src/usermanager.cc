/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usermanager.cc                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncastell <ncastell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 01:44:03 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/25 19:14:15 by ncastell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include "usermanager.h"

UserManager::UserManager(void)
{
}

UserManager::~UserManager(void)
{
	std::map<int, User *>::iterator it(users.begin());
	for (; it != users.end(); it++)
		delete (it->second);
}

void UserManager::AddUser(int fd)
{
	users[fd] = new User(fd);
	Server::instance->events().AddEvent<Event::Client>(fd, POLLIN);
}

void UserManager::DelUser(int fd)
{
	Log() << "Connection " << fd << " closed";
	delete (users[fd]);
	users.erase(fd);
	Server::instance->events().DelEvent(fd);
}

User *UserManager::Search(const std::string &name)
{
	std::map<int, User *>::iterator it(users.begin());
	for (; it != users.end(); it++)
	{
		if ((it->second)->nickname() == name)
			return (it->second);
	}
	return (0);
}

void UserManager::Read(void)
{
	std::map<int, User *>::iterator it(users.begin());
	while (it != users.end())
	{
		User *user(it->second);
		++it;
		if (!user->rbuf().empty())
			user->Request();
	}
}

void UserManager::Read(int fd)
{
	users[fd]->Read();
}

void UserManager::Write(void)
{
	std::map<int, User *>::iterator it(users.begin());
	while (it != users.end())
	{
		User *user(it->second);
		it++;
		if (!user->wbuf().empty())
			Server::instance->events().MaskEvent(user->socket(), POLLOUT);
	}
}

void UserManager::Write(int fd)
{
	users[fd]->Write();
	Server::instance->events().UnmaskEvent(fd, POLLOUT);
}

void UserManager::CheckTimeouts(void)
{
	time_t now(time(NULL));
	std::map<int, User *>::iterator it(users.begin());
	while (it != users.end())
	{
		User *user(it->second);
		++it;
		if (!user->registered() && user->last_activity() < now - TIMEOUT)
		{
			Log() << "Unregistered connection " << user->socket() << " timed out";
			DelUser(user->socket());
		}
	}
}
