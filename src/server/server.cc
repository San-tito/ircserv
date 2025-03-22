/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 20:28:37 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/22 20:46:28 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

Server *Server::instance(0);

Server::Server(int port, std::string password)
{
	instance = this;
	(void)port;
	(void)password;
}

Server::~Server(void)
{
}

void Server::Exit(int status)
{
	instance = 0;
	delete (this);
	exit(status);
}
