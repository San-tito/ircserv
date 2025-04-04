/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientmanager.cc                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncastell <ncastell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 01:44:03 by sguzman           #+#    #+#             */
/*   Updated: 2025/04/01 18:27:18 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "clientmanager.h"
#include "server.h"

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

void ClientManager::CloseClient(int fd, const std::string &message)
{
	Client	*client;

	client = clients[fd];
	client->set_closing(true);
	client->Write("ERROR :Closing connection: " + message);
	Log() << "Connection " << fd << " closed: " << message;
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
			CloseClient(client->socket(), "timed out");
	}
}

void ClientManager::Close(void)
{
	std::map<int, Client *>::iterator it(clients.begin());
	while (it != clients.end())
	{
		Client *client(it->second);
		it++;
		if (client->closing())
		{
			Server::instance->events().DelEvent(client->socket());
			clients.erase(client->socket());
			delete (client);
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
