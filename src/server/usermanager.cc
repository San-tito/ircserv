/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usermanager.cc                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 01:44:03 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/23 13:38:39 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
