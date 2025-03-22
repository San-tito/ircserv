/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 20:28:37 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/22 23:01:07 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

Server *Server::instance(0);

Server::Server(int port, std::string password) : password(password)
{
	instance = this;
	this->startup_time = time(0);
	SetSignals();
	this->listener = new Listener(port);
}

Server::~Server(void)
{
	delete (this->listener);
}

void Server::Run(void)
{
	while (true)
		;
}

void Server::Exit(int status)
{
	instance = 0;
	delete (this);
	exit(status);
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
		Server::instance->Exit(EXIT_SUCCESS);
		break ;
	case SIGTERM:
	case SIGQUIT:
		Server::instance->Exit(EXIT_FAILURE);
		break ;
	}
}
