/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 20:28:00 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/22 23:27:55 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

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
	Server(int port, std::string password);
	~Server(void);

	void Run(void);
	void Exit(int status);

	time_t startup_time;
	std::string password;
	Listener *listener;

	static Server *instance;

  private:
	void SetSignals(void);

	static void SignalHandler(int sig);
};

#endif /* IRCD_HPP */
