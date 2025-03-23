/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listener.cc                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 23:08:49 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/23 02:59:37 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "listener.h"

Listener::Listener(int port)
{
	if (!InitAddress(port))
	{
		Log() << "Can't listen on [" << LISTEN_ADDRESS << "]:" << port << ": Failed to parse IP address!";
		Server::instance->Exit(EXIT_FAILURE);
	}
	int af(this->address_.sin_family);
	this->fd_ = socket(af, SOCK_STREAM, 0);
	if (this->fd_ == -1)
	{
		Log() << "Can't create socket (af " << af << ") : " << strerror(errno) << '!';
		Server::instance->Exit(EXIT_FAILURE);
	}
	if (fcntl(this->fd_, F_SETFL, O_NONBLOCK) != 0)
	{
		close(this->fd_);
		Log() << "Can't enable non-blocking mode for socket: " << strerror(errno) << '!';
		Server::instance->Exit(EXIT_FAILURE);
	}
	if (bind(this->fd_, reinterpret_cast<struct sockaddr *>(&this->address_),
			sizeof(this->address_)) != 0)
	{
		close(this->fd_);
		Log() << "Can't bind socket to [" << LISTEN_ADDRESS << "]:" << port << ": " << strerror(errno) << '!';
		Server::instance->Exit(EXIT_FAILURE);
	}
	if (listen(this->fd_, 10) != 0)
	{
		close(this->fd_);
		Log() << "Can't listen on [" << LISTEN_ADDRESS << "]:" << port << ": " << strerror(errno) << '!';
		Server::instance->Exit(EXIT_FAILURE);
	}
	Server::instance->events()->AddEvent(this->fd_, POLLIN, new Event::Server);
	Log() << "Listening on [" << LISTEN_ADDRESS << "]:" << port << " ...";
}

Listener::~Listener(void)
{
	close(this->fd_);
	Log() << "Listener closed.";
}

bool Listener::InitAddress(int port)
{
	bzero(&this->address_, sizeof(this->address_));
	this->address_.sin_family = AF_INET;
	this->address_.sin_addr.s_addr = inet_addr(LISTEN_ADDRESS);
	if (this->address_.sin_addr.s_addr == (unsigned)-1)
		return (false);
	this->address_.sin_port = htons(port);
	return (true);
}
