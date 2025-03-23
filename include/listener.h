/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listener.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 23:02:45 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/23 01:09:42 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENER_H
# define LISTENER_H

# include "server.h"
# include <arpa/inet.h>
# include <cerrno>
# include <cstdlib>
# include <cstring>
# include <fcntl.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <unistd.h>

# define LISTEN_ADDRESS "0.0.0.0"

class Listener
{
  public:
	Listener(int port);
	~Listener(void);

	bool InitAddress(int port);
	bool InitSocket(void);

  private:
	int sock_;
	struct sockaddr_in address_;
};

#endif /* LISTENER_H */
