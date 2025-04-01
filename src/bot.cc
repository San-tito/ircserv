/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cc                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncastell <ncastell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 20:28:37 by sguzman           #+#    #+#             */
/*   Updated: 2025/04/01 19:45:13 by ncastell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bot.h"

Bot *Bot::instance(0);

Bot::Bot(std::string host, int port, std::string password)
{
	instance = this;
	SetSignals();
	InitConnection(port, host);
	Authenticate(password);
}

Bot::~Bot(void)
{
	close(this->sock_);
}

void Bot::InitConnection(int port, std::string &host)
{
	std::string listen_addr(host);
	struct hostent *h(gethostbyname(host.c_str()));
	if (h)
		listen_addr = h->h_name;
	if (!Tool::InitAddress(&this->address_, port, listen_addr.c_str()))
	{
		Log() << "Can't connect on [" << listen_addr << "]:" << port << ": Failed to parse IP address!";
		Exit(EXIT_FAILURE);
	}
	int af(this->address_.sin_family);
	this->sock_ = socket(af, SOCK_STREAM, 0);
	if (this->sock_ == -1)
	{
		Log() << "Can't create socket (af " << af << ") : " << strerror(errno) << '!';
		Exit(EXIT_FAILURE);
	}
	if (connect(this->sock_,
			reinterpret_cast<struct sockaddr *>(&this->address_),
			sizeof(this->address_)) != 0)
	{
		Log() << "Can't connect to [" << listen_addr << "]:" << port << ": " << strerror(errno) << '!';
		Exit(EXIT_FAILURE);
	}
}

void Bot::Authenticate(std::string password)
{
	if (!password.empty())
		Write("PASS " + password);
	Write("NICK " + NICKNAME);
	Write("USER " + USERNAME + " 0 * :realname");
}

void Bot::Read(void)
{
	ssize_t	len;
	char	buf[READBUFFER_LEN];

	len = read(this->sock_, buf, READBUFFER_LEN);
	if (len == 0)
	{
		Log() << "Server close connection";
		Exit(EXIT_FAILURE);
	}
	if (len < 0)
	{
		Log() << "Read error: " << strerror(errno) << '!';
		Exit(EXIT_FAILURE);
	}
	buf[len] = '\0';
	std::string msg(buf);
	ParseInstruction(msg);
}

void Bot::Write(std::string const &msg)
{
	ssize_t	len;

	std::string buff(msg + "\r\n");
	len = write(this->sock_, buff.c_str(), buff.size());
	if (len < 0)
	{
		Log() << "Write error: " << strerror(errno) << '!';
		Exit(EXIT_FAILURE);
	}
}

void Bot::Run(void)
{
	while (true)
	{
		Read();
	}
}

void Bot::Exit(int status)
{
	Log() << "Bot exiting";
	instance = 0;
	delete (this);
	exit(status);
}

void Bot::SetSignals(void)
{
	signal(SIGINT, Bot::SignalHandler);
	signal(SIGTERM, Bot::SignalHandler);
	signal(SIGQUIT, Bot::SignalHandler);
}

void Bot::SignalHandler(int sig) { Log() << "Received signal: \"" << strsignal(sig) << '\"'; switch (sig) { case SIGINT: case SIGTERM: case SIGQUIT: Bot::instance->Exit(EXIT_SUCCESS); break ; }
}

void Bot::Trim(std::string &str)
{
	size_t start(str.find_first_not_of(" \t\r\n"));
	size_t end(str.find_last_not_of(" \t\r\n"));
	if (start == std::string::npos || end == std::string::npos)
		str.clear();
	else
		str = str.substr(start, end - start + 1);
}

std::vector<std::string>	Bot::userList(std::string users)
{
	std::vector<std::string>	users_;

	size_t	pos;

	Trim(users);
	while (!users.empty())
	{
		pos = users.find(',');
		if (pos != std::string::npos)
		{
			users_.push_back(users.substr(0, pos));
			users = users.substr(pos + 1);
		}
		else
		{
			users_.push_back(users);
			break ;
		}
	}
	return (users_);
}

void Bot::executeAction(std::string &action, std::vector<std::string> users, std::string &msg)
{
	if (action == "!msg")
	{
	  for (std::vector<std::string>::iterator it = users.begin(); it != users.end(); ++it)
		Write("PRIVMSG " + *it + " :" + msg);
	}
	else if (action == "!laugh") 
	{
	  for (std::vector<std::string>::iterator it = users.begin(); it != users.end(); ++it) {

		std::string command = "curl -H \"Accept: text/plain\" https://icanhazdadjoke.com/";
		std::ostringstream joke;

		joke << system(command.c_str()) << '\n';

		Write("PRIVMSG " + *it + " :" + joke.str());
	  }
	}
}

void Bot::ParseInstruction(std::string& request)
{
	std::string     action;
	std::string     users;
	std::string     msg;

	Trim(request);
	size_t  pos = request.find(' ');
	if (pos != std::string::npos)
	{
			action = request.substr(0, pos);
			request = request.substr(pos + 1);
	}
	Trim(request);
	pos = request.find(' ');
	if (pos != std::string::npos)
	{
			users = request.substr(0, pos);
			request = request.substr(pos + 1);
	}
	Trim(request);
	msg = request;
	std::cout << "ACTION = " << action << std::endl;
	std::cout << "USERS = " << users << std::endl;
	std::cout << "MSG= " << msg << std::endl;
	/*TODO*/
}

void Bot::ParseParams(std::string &request, std::vector<std::string> &params)
{
	size_t  pos;

	Trim(request);
	while (!request.empty())
	{
			if (request[0] == ':')
			{
					params.push_back(request.substr(1));
					break ;
			}
			pos = request.find(' ');
			if (pos != std::string::npos)
			{
					params.push_back(request.substr(0, pos));
					request = request.substr(pos + 1);
					Trim(request);
			}
			else
			{
					params.push_back(request);
					break ;
			}
	}
}

bool    Bot::ParseCmd(std::string& request, std::string& command)
{
        size_t  pos;

        Trim(request);
        pos = request.find(' ');
        if (request[0] == ':')
        {
                if (pos == std::string::npos)
                        return (false);
                request = request.substr(pos + 1);
        }
        pos = request.find(' ');
        if (pos != std::string::npos)
        {
                command = request.substr(0, pos);
                request = request.substr(pos + 1);
        }
        else
        {
                command = request;
                request.clear();
        }
        return (true);
}

void    Bot::Parser(std::string request)
{
        std::string     command("");
        std::vector<std::string> params;

        if (!ParseCmd(request, command))
        {
                std::cerr << "ERROR :Prefix without command.";
                return ;
        }
        ParseParams(request, params);
        if (params.size() > MAX_PARAMS)
        {
                std::cerr << "ERROR : Too much params.";
                return ;
        }
        std::cout << "COMMAND: " << command << std::endl;
        std::cout << "USERS: " << params[0] << std::endl;
        std::cout << "BOT MSG: " << params[1] << std::endl;
        ParseAction(params[1]);
}

void	Bot::ParseAction(std::string& request)
{
	std::string     action;
	std::string     users;
	std::string     msg;

	Trim(request);
	size_t  pos = request.find(' ');
	if (pos != std::string::npos)
	{
			action = request.substr(0, pos);
			request = request.substr(pos + 1);
	}
	Trim(request);
	pos = request.find(' ');
	if (pos != std::string::npos)
	{
			users = request.substr(0, pos);
			request = request.substr(pos + 1);
	}
	Trim(request);
	msg = request;

	std::cout << "ACTION = " << action << std::endl;
	std::cout << "USERS = " << users << std::endl;
	std::cout << "MSG= " << msg << std::endl;
	/*TODO*/
}

int	main(int argc, char **argv)
{
	int	port;

	if (argc != 4)
	{
		std::cerr << "Usage: " << argv[0] << " <host> <port> <password>\n";
		return (EXIT_FAILURE);
	}
	port = Tool::ParsePort(argv[2]);
	if (port < 0)
	{
		std::cerr << "illegal port number " << argv[2] << "!\n";
		return (EXIT_FAILURE);
	}
	new Bot(argv[1], port, argv[3]);
	Bot::instance->Run();
	return (EXIT_SUCCESS);
}
