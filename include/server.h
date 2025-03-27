/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncastell <ncastell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 20:28:00 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/27 14:41:23 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# include "channelmanager.h"
# include "clientmanager.h"
# include "eventmanager.h"
# include "listener.h"
# include "logging.h"
# include "parser.h"
# include "typedefs.h"
# include <algorithm>
# include <cfloat>
# include <climits>
# include <csignal>
# include <cstdarg>
# include <cstring>
# include <map>
# include <netdb.h>
# include <set>
# include <string>
# include <vector>

# define LISTEN_ADDRESS "0.0.0.0"

class Server
{
  public:
	static Server *instance;

	Server(int port, std::string password);
	~Server(void);

	void Run(void);
	void Exit(int status);

	std::string startup_time(void) const;
	std::string password(void) const;
	std::string servername(void) const;
	ClientManager &clients(void);
	EventManager &events(void);
	CommandParser &parser(void);
	ChannelManager &channels(void);

  private:
	void SetSignals(void);
	static void SignalHandler(int sig);

	time_t startup_time_;
	std::string password_;
	std::string servername_;
	Listener *listener_;
	ClientManager clients_;
	EventManager events_;
	CommandParser parser_;
	ChannelManager channels_;
};

#endif /* SERVER_H */
