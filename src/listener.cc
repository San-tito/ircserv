/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listener.cc                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 23:08:49 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/27 16:19:25 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "listener.h"

Listener::Listener(int port, std::string &listen_addr)
{
	if (!Connection::InitAddress(&this->address_, port, listen_addr.c_str()))
	{
		Log() << "Can't listen on [" << listen_addr << "]:" << port << ": Failed to parse IP address!";
		Server::instance->Exit(EXIT_FAILURE);
	}
	int af(this->address_.sin_family);
	this->sock_ = socket(af, SOCK_STREAM, 0);
	if (this->sock_ == -1)
	{
		Log() << "Can't create socket (af " << af << ") : " << strerror(errno) << '!';
		Server::instance->Exit(EXIT_FAILURE);
	}
	if (!Connection::InitSocket(this->sock_))
		Server::instance->Exit(EXIT_FAILURE);
	if (bind(this->sock_, reinterpret_cast<struct sockaddr *>(&this->address_),
			sizeof(this->address_)) != 0)
	{
		close(this->sock_);
		Log() << "Can't bind socket to [" << LISTEN_ADDRESS << "]:" << port << ": " << strerror(errno) << '!';
		Server::instance->Exit(EXIT_FAILURE);
	}
	if (listen(this->sock_, 10) != 0)
	{
		close(this->sock_);
		Log() << "Can't listen on [" << LISTEN_ADDRESS << "]:" << port << ": " << strerror(errno) << '!';
		Server::instance->Exit(EXIT_FAILURE);
	}
	Server::instance->events().AddNewConnection(this->sock_, POLLIN);
	Log() << "Server listening on [" << LISTEN_ADDRESS << "]:" << port;
}

Listener::~Listener(void)
{
	close(this->sock_);
	Log() << "Listener closed.";
}
