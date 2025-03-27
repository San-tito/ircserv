/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 23:02:45 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/27 19:32:45 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_H
# define CONNECTION_H

# include "logging.h"
# include <arpa/inet.h>
# include <cerrno>
# include <cstdlib>
# include <cstring>
# include <fcntl.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <unistd.h>

class Connection
{
  public:
	static int ParsePort(char *arg);
	static bool InitSocket(int socket);
	static bool InitAddress(struct sockaddr_in *addr, int port,
		const char *listen_addr);
};

#endif /* CONNECTION_H */
