/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool.cc                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 23:08:49 by sguzman           #+#    #+#             */
/*   Updated: 2025/04/03 01:20:17 by tuta             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tool.h"

void Tool::Trim(std::string &str)
{
	size_t start(str.find_first_not_of(" \t\r\n"));
	size_t end(str.find_last_not_of(" \t\r\n"));
	if (start == std::string::npos || end == std::string::npos)
		str.clear();
	else
		str = str.substr(start, end - start + 1);
}

int Tool::ParsePort(char *arg)
{
	char	*endptr;

	int port(strtol(arg, &endptr, 10));
	if (port > 0 && port < 0xFFFF && *endptr == '\0')
		return (port);
	return (-1);
}
bool Tool::InitAddress(struct sockaddr_in *addr, int port,
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

bool Tool::InitSocket(int socket)
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

std::string Tool::ToUpperCase(const std::string &str) {
    std::string result = str;
    for (size_t i = 0; i < result.length(); ++i) {
        result[i] = std::toupper(result[i]);
    }
    return result;
}
