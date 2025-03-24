/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cc                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 20:28:37 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/24 21:26:38 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bot.h"

Bot *Bot::instance(0);

Bot::Bot(std::string const &host, int port) : host_(host), port_(port)
{
	instance = this;
	SetSignals();
}

Bot::~Bot(void)
{
}

void Bot::Run(void)
{
	while (true)
	{
		// do something
	}
}

void Bot::Exit(int status)
{
	Log() << "Bot exiting";
	instance = 0;
	delete (this);
	exit(status);
}

void Bot::SetSignals(void)
{
	signal(SIGINT, Bot::SignalHandler);
	signal(SIGTERM, Bot::SignalHandler);
	signal(SIGQUIT, Bot::SignalHandler);
}

void Bot::SignalHandler(int sig)
{
	Log() << "Received signal: \"" << strsignal(sig) << '\"';
	switch (sig)
	{
	case SIGINT:
	case SIGTERM:
	case SIGQUIT:
		Bot::instance->Exit(EXIT_SUCCESS);
		break ;
	}
}

int	ParsePort(char *arg)
{
	char	*endptr;

	int port(strtol(arg, &endptr, 10));
	if (port > 0 && port < 0xFFFF && *endptr == '\0')
		return (port);
	return (-1);
}

int	main(int argc, char **argv)
{
	int	port;

	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <host> <port>\n";
		return (EXIT_FAILURE);
	}
	port = ParsePort(argv[1]);
	if (port < 0)
	{
		std::cerr << "illegal port number " << argv[1] << "!\n";
		return (EXIT_FAILURE);
	}
	new Bot(argv[1], port);
	Bot::instance->Run();
	return (EXIT_SUCCESS);
}
