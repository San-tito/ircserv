/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event.cc                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 02:31:03 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/23 02:57:25 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "event.h"

Event::~Event(void)
{
}

Event::Server::Server(int socket) : socket_(socket)
{
}

Event::Server::~Server(void)
{
}

void Event::Server::Callback(short revents)
{
	static_cast<void>(revents);
	Log() << "Accepting new connection ...";
	int new_socket(accept(this->socket_, 0, 0));
	if (new_socket < 0)
	{
		Log() << "Can't accept connection: " << strerror(errno) << '!';
		::Server::instance->Exit(EXIT_FAILURE);
	}
	::Server::instance->events()->AddEvent(new_socket, POLLIN,
		new Event::Client);
}

Event::Client::Client(void)
{
}

Event::Client::~Client(void)
{
}

void Event::Client::Callback(short revents)
{
	(void)revents;
}
