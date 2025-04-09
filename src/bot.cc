/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cc                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncastell <ncastell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 20:28:37 by sguzman           #+#    #+#             */
/*   Updated: 2025/04/09 14:48:59 by naomy            ###   ########.fr       */
/*   Updated: 2025/04/07 15:48:11 by ncastell         ###   ########.fr       */
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

std::string Bot::Read(void)
{
	ssize_t	len;
	char	buf[READBUFFER_LEN];

	len = recv(this->sock_, buf, READBUFFER_LEN, 0);
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
	return (buf);
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
		std::string buf(Read());
		Parser(buf);
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

void Bot::SignalHandler(int sig)
{
	Log() << "Received signal: \"" << strsignal(sig) << '\"';
	switch (sig)
	{
	case SIGINT:
	case SIGTERM:
	case SIGQUIT:
		Bot::instance->Exit(EXIT_SUCCESS);
		break ;
	}
}

void Bot::ParseParams(std::string &request, std::vector<std::string> &params)
{
	size_t	pos;

	Tool::Trim(request);
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
			Tool::Trim(request);
		}
		else
		{
			params.push_back(request);
			break ;
		}
	}
}

bool Bot::ParseCmd(std::string &request, std::string &command)
{
	size_t	pos;

	Tool::Trim(request);
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

void Bot::Parser(std::string request)
{
	std::string command;
	std::vector<std::string> params;
	std::string sender_nick = request.substr(1, request.find('!') - 1);
	if (!ParseCmd(request, command))
		return (this->Write("ERROR :Prefix without command."));
	ParseParams(request, params);
	if (params.size() > MAX_PARAMS)
		return (Write("ERROR : Too many params."));
	if (command == "ERROR")
		Log() << params[0];
	else if (command == "PRIVMSG")
		ParseAction(params[1], sender_nick);
}

void Bot::ParseAction(std::string &request, std::string &nickname)
{
	std::string action;
	std::vector<std::string> params;
	std::string users;
	std::string message = "";
	if (!ParseCmd(request, action))
		return (Write("ERROR :Prefix without command."));
	ParseParams(request, params);
	if ((params.size() != PARAMS_MSG) && action == "!msg")
		return (Write("PRIVMSG " + nickname
				+ " : USAGE: !msg <users>/<user1,user2,...> <message>/:<message>"));
	else if ((params.size() > PARAMS_JOKE) && action == "!joke")
		return (Write("PRIVMSG " + nickname
				+ " : USAGE: !joke <users>/<user1,user2,...>\n USAGE: !joke"));
	if (action == "!msg")
		message = params[1];
	if (params.empty())
		users = nickname;
	else
		users = params[0];
	executeAction(action, users, message, nickname);
}

void Bot::executeAction(std::string &action, std::string &users,
	std::string &msg, std::string &sender)
{
	if (action == "!joke")
		msg = Jokes::getRandomJoke();
	else if (action == "!help")
	{
		Write("PRIVMSG " + sender + " :!msg <users>/<user1,user2,...> :<message> - Send an anonymous message a user or a list of users");
		Write("PRIVMSG " + sender + " :!joke - Get a random joke");
		Write("PRIVMSG " + sender + " :!joke <users>/<user1,user2,...> - Send a joke to a user or a list of users");
		Write("PRIVMSG " + sender + " :!help - Display this help message");
		return ;
	}
	else if (action == "!msg")
		msg = "An anonymous user said: \"\037" + msg + "\037\"";
	else
	{
		Write("PRIVMSG " + sender + " :Command not found. !help for help");
		return ;
	}
	Write("PRIVMSG " + users + " :" + msg);
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
	std::srand(std::time(0));
	new Bot(argv[1], port, argv[3]);
	Bot::instance->Run();
	return (EXIT_SUCCESS);
}
