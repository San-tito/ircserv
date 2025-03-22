/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cc                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 20:20:19 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/22 20:48:12 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

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
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password>\n";
		return (EXIT_FAILURE);
	}
	int port(ParsePort(argv[1]));
	if (port < 0)
	{
		std::cerr << "illegal port number " << argv[1] << "!\n";
		return (EXIT_FAILURE);
	}
	new Server(port, argv[2]);
	// Server::instance->Run();
	delete Server::instance;
	return (EXIT_SUCCESS);
}
