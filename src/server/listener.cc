/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listener.cc                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 23:08:49 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/22 23:29:06 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "listener.h"

Listener::Listener(int port) : port(port)
{
	if (!this->InitAddress())
	{
		Log() << "Can't listen on [" << LISTEN_ADDRESS << "]:" << port << ": Failed to parse IP address!";
		Server::instance->Exit(EXIT_FAILURE);
	}
	int af(this->address.sin_family);
	this->sock = socket(af, SOCK_STREAM, 0);
	if (this->sock == -1)
	{
		Log() << "Can't create socket (af " << af << ") : " << strerror(errno) << '!';
		Server::instance->Exit(EXIT_FAILURE);
	}
	if (fcntl(this->sock, F_SETFL, O_NONBLOCK) != 0)
	{
		close(this->sock);
		Log() << "Can't enable non-blocking mode for socket: " << strerror(errno) << '!';
		Server::instance->Exit(EXIT_FAILURE);
	}
	if (bind(this->sock, reinterpret_cast<struct sockaddr *>(&this->address),
			sizeof(this->address)) != 0)
	{
		close(this->sock);
		Log() << "Can't bind socket to [" << LISTEN_ADDRESS << "]:" << port << ": " << strerror(errno) << '!';
		Server::instance->Exit(EXIT_FAILURE);
	}
	if (listen(this->sock, 10) != 0)
	{
		close(this->sock);
		Log() << "Can't listen on [" << LISTEN_ADDRESS << "]:" << port << ": " << strerror(errno) << '!';
		Server::instance->Exit(EXIT_FAILURE);
	}
	Log() << "Listening on [" << LISTEN_ADDRESS << "]:" << port << " ...";
}

Listener::~Listener(void)
{
	close(this->sock);
	Log() << "Listening socket " << this->sock << " closed.";
}

bool Listener::InitAddress(void)
{
	bzero(&this->address, sizeof(this->address));
	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = inet_addr(LISTEN_ADDRESS);
	if (this->address.sin_addr.s_addr == (unsigned)-1)
		return (false);
	this->address.sin_port = htons(this->port);
	return (true);
}
