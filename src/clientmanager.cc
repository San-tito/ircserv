/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientmanager.cc                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncastell <ncastell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 01:44:03 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/26 21:46:32 by ncastell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include "clientmanager.h"

ClientManager::ClientManager(void)
{
}

ClientManager::~ClientManager(void)
{
	std::map<int, Client *>::iterator it(clients.begin());
	for (; it != clients.end(); it++)
		delete (it->second);
}

void ClientManager::AddClient(int fd)
{
	clients[fd] = new Client(fd);
	Server::instance->events().AddClientSession(fd, POLLIN);
}

void ClientManager::DelClient(int fd)
{
	Log() << "Connection " << fd << " closed";
	delete (clients[fd]);
	clients.erase(fd);
	Server::instance->events().DelEvent(fd);
}

Client *ClientManager::Search(const std::string &name)
{
	std::map<int, Client *>::iterator it(clients.begin());
	for (; it != clients.end(); it++)
	{
		if ((it->second)->nickname() == name)
			return (it->second);
	}
	return (0);
}

void ClientManager::Read(void)
{
	std::map<int, Client *>::iterator it(clients.begin());
	while (it != clients.end())
	{
		Client *client(it->second);
		++it;
		if (!client->rbuf().empty())
			client->Request();
	}
}

void ClientManager::Read(int fd)
{
	clients[fd]->Read();
}

void ClientManager::Write(void)
{
	std::map<int, Client *>::iterator it(clients.begin());
	while (it != clients.end())
	{
		Client *client(it->second);
		it++;
		if (!client->wbuf().empty())
			Server::instance->events().MaskEvent(client->socket(), POLLOUT);
	}
}

void ClientManager::Write(int fd)
{
	clients[fd]->Write();
	Server::instance->events().UnmaskEvent(fd, POLLOUT);
}

void ClientManager::CheckTimeouts(void)
{
	time_t now(time(NULL));
	std::map<int, Client *>::iterator it(clients.begin());
	while (it != clients.end())
	{
		Client *client(it->second);
		++it;
		if (!client->registered() && client->last_activity() < now - TIMEOUT)
		{
			Log() << "Unregistered connection " << client->socket() << " timed out";
			DelClient(client->socket());
		}
	}
}

bool ClientManager::IsValidNick(std::string nick)
{
	if (nick.size() > MAX_NICK_LEN)
		return (false);
	if (nick.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]\\`_^{|}-") != std::string::npos)
		return (false);
	return (true);
}
