/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncastell <ncastell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 20:28:00 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/31 18:12:08 by ncastell         ###   ########.fr       */
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

# define READBUFFER_LEN 2048
# define NICKNAME std::string("whisper")
# define USERNAME std::string("whisper")

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
	void Read(void);
	void Write(std::string const &msg);

	/*Parsing*/
	// PRIVMSG bot "/bot user1 :message"
	void	Trim(std::string &str);
	void	parseInstruction(std::string& request);
	std::vector<std::string>	Bot::userList(std::string users);
	void	SaveUsers(std::vector<std::string>& users, std::string& ustr);

	int sock_;
	struct sockaddr_in address_;	
};

#endif /* BOT_H */
