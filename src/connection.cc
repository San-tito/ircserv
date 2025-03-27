/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection.cc                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 23:08:49 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/27 19:33:11 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "connection.h"

int Connection::ParsePort(char *arg)
{
	char	*endptr;

	int port(strtol(arg, &endptr, 10));
	if (port > 0 && port < 0xFFFF && *endptr == '\0')
		return (port);
	return (-1);
}
bool Connection::InitAddress(struct sockaddr_in *addr, int port,
	const char *listen_addr)
{
	bzero(addr, sizeof(addr));
	addr->sin_family = AF_INET;
	addr->sin_addr.s_addr = inet_addr(listen_addr);
	if (addr->sin_addr.s_addr == (unsigned)-1)
		return (false);
	addr->sin_port = htons(port);
	return (true);
}

bool Connection::InitSocket(int socket)
{
	if (fcntl(socket, F_SETFL, O_NONBLOCK) != 0)
	{
		close(socket);
		Log() << "Can't enable non-blocking mode for new socket: " << strerror(errno) << '!';
		return (false);
	}
	int value(1);
	if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &value,
			sizeof(value)) != 0)
		Log() << "Can't set SO_REUSEADDR for new socket: " << strerror(errno) << '!';
	return (true);
}
