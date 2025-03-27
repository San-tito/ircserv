/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 20:28:00 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/27 17:09:10 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_H
# define BOT_H

# include "connection.h"
# include "logging.h"
# include <algorithm>
# include <cfloat>
# include <climits>
# include <csignal>
# include <cstdarg>
# include <cstring>
# include <iostream>
# include <map>
# include <netdb.h>
# include <set>
# include <sstream>
# include <string>
# include <vector>

# define NICKNAME std::string("bot")
# define USERNAME std::string("bot")

class Bot
{
  public:
	static Bot *instance;

	Bot(std::string host, int port, std::string password);
	~Bot(void);

	void Run(void);

  private:
	static void SignalHandler(int sig);
	void Exit(int status);
	void SetSignals(void);
	void InitConnection(int port, std::string &listen_addr);
	void Authenticate(std::string password);
	void Write(std::string const &msg);

	int sock_;
	struct sockaddr_in address_;
};

#endif /* BOT_H */
