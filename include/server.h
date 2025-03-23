/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 20:28:00 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/23 02:57:11 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "eventmanager.h"
# include "listener.h"
# include "logging.h"
# include "typedefs.h"
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
	EventManager *events(void) const;

  private:
	void SetSignals(void);
	static void SignalHandler(int sig);

	time_t startup_time_;
	std::string password_;
	Listener *listener_;
	EventManager events_;
};

#endif /* IRCD_HPP */
