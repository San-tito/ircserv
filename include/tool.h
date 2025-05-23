/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 23:02:45 by sguzman           #+#    #+#             */
/*   Updated: 2025/04/03 01:20:08 by tuta             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOOL_H
# define TOOL_H

# include "logging.h"
# include <arpa/inet.h>
# include <cerrno>
# include <cstdlib>
# include <cstring>
# include <fcntl.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <unistd.h>

class Tool
{
  public:
	static void Trim(std::string &str);
	static int ParsePort(char *arg);
	static bool InitSocket(int socket);
	static bool InitAddress(struct sockaddr_in *addr, int port,
		const char *listen_addr);
	static std::string ToUpperCase(const std::string &str);
};

#endif /* CONNECTION_H */
