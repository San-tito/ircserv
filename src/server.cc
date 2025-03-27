/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncastell <ncastell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 20:28:37 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/27 14:42:44 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

Server *Server::instance(0);

Server::Server(int port, std::string password) : password_(password)
{
	instance = this;
	struct hostent *h(0);
	h = gethostbyname(LISTEN_ADDRESS);
	if (h)
		this->servername_ = h->h_name;
	this->startup_time_ = time(0);
	this->listener_ = new Listener(port);
	SetSignals();
}

Server::~Server(void)
{
	delete (this->listener_);
}

void Server::Run(void)
{
	while (true)
	{
		clients_.CheckTimeouts();
		clients_.Read();
		clients_.Write();
		events_.Dispatch();
	}
}

void Server::Exit(int status)
{
	Log() << "Server shutting down...";
	instance = 0;
	delete (this);
	Log() << "Server shut down.";
	exit(status);
}

std::string Server::password(void) const
{
	return (this->password_);
}

std::string Server::startup_time(void) const
{
	char time[64];

	strftime(time, sizeof(time), "%a %b %d %Y at %H:%M:%S",
		localtime(&this->startup_time_));
	return (time);
}

std::string Server::servername(void) const
{
	return (this->servername_);
}

EventManager &Server::events(void)
{
	return (this->events_);
}

ClientManager &Server::clients(void)
{
	return (this->clients_);
}

CommandParser &Server::parser(void)
{
	return (this->parser_);
}

ChannelManager &Server::channels(void)
{
	return (this->channels_);
}

void Server::SetSignals(void)
{
	signal(SIGINT, Server::SignalHandler);
	signal(SIGTERM, Server::SignalHandler);
	signal(SIGQUIT, Server::SignalHandler);
}

void Server::SignalHandler(int sig)
{
	Log() << "Received signal: \"" << strsignal(sig) << '"';
	switch (sig)
	{
	case SIGINT:
	case SIGTERM:
	case SIGQUIT:
		Server::instance->Exit(EXIT_SUCCESS);
		break ;
	}
}
