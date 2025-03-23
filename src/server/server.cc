/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 20:28:37 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/23 01:28:26 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

Server *Server::instance(0);

Server::Server(int port, std::string password) : password_(password)
{
	instance = this;
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
		;
}

void Server::Exit(int status)
{
	Log() << "Server shutting down!";
	instance = 0;
	delete (this);
	exit(status);
}

std::string Server::password(void) const
{
	return (this->password_);
}

time_t Server::startup_time(void) const
{
	return (this->startup_time_);
}

void Server::SetSignals(void)
{
	signal(SIGINT, Server::SignalHandler);
	signal(SIGTERM, Server::SignalHandler);
	signal(SIGQUIT, Server::SignalHandler);
}

void Server::SignalHandler(int sig)
{
	Log() << "Got signal \"" << strsignal(sig) << "\" ...";
	switch (sig)
	{
	case SIGINT:
	case SIGTERM:
	case SIGQUIT:
		Server::instance->Exit(EXIT_SUCCESS);
		break ;
	}
}
