/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cc                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 20:20:19 by sguzman           #+#    #+#             */
/*   Updated: 2025/04/01 11:34:48 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

int	main(int argc, char **argv)
{
	int	port;

	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password>\n";
		return (EXIT_FAILURE);
	}
	port = Tool::ParsePort(argv[1]);
	if (port < 0)
	{
		std::cerr << "illegal port number " << argv[1] << "!\n";
		return (EXIT_FAILURE);
	}
	new Server(port, argv[2]);
	Server::instance->Run();
	delete Server::instance;
	return (EXIT_SUCCESS);
}
