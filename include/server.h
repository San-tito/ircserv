/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 20:28:00 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/26 15:46:47 by tuta             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# include "channelmanager.h"
# include "eventmanager.h"
# include "listener.h"
# include "logging.h"
# include "parser.h"
# include "typedefs.h"
# include "usermanager.h"
# include <algorithm>
# include <cfloat>
# include <climits>
# include <csignal>
# include <cstdarg>
# include <cstring>
# include <map>
# include <set>
# include <string>
# include <vector>

class Server
{
  public:
	static Server *instance;

	Server(int port, std::string password);
	~Server(void);

	void Run(void);
	void Exit(int status);

	std::string password(void) const;
	time_t startup_time(void) const;
	UserManager &users(void);
	EventManager &events(void);
	CommandParser &parser(void);
	ChannelManager &channels(void);

  private:
	void SetSignals(void);
	static void SignalHandler(int sig);

	time_t startup_time_;
	std::string password_;
	Listener *listener_;
	UserManager users_;
	EventManager events_;
	CommandParser parser_;
	ChannelManager channels_;
};

#endif /* SERVER_H */
