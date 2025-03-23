/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usermanager.cc                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 01:44:03 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/23 14:18:04 by sguzman          ###   ########.fr       */
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
	Log() << "Connection " << fd << " closed\n";
	delete (users[fd]);
	users.erase(fd);
	Server::instance->events().DelEvent(fd);
}

void UserManager::Read(int fd)
{
	users[fd]->Read();
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
	for (; it != users.end(); it++)
	{
		User *user(it->second);
		if (!user->registered() && user->last_activity() < now - TIMEOUT)
		{
			Log() << "Unregistered connection " << user->socket() << " timed out";
			DelUser(user->socket());
		}
	}
}
