/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncastell <ncastell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 15:38:10 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/26 17:27:22 by ncastell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "server.h"

CommandParser::CommandParser(void)
{
	commands_["INVITE"] = new Invite;
	commands_["JOIN"] = new Join;
	// commands_["KICK"] = new Command::Kick;
	// commands_["MODE"] = new Command::Mode;
	commands_["NICK"] = new Nick;
	// commands_["PART"] = new Command::Part;
	commands_["PASS"] = new Pass;
	// commands_["PRIVMSG"] = new Command::Privmsg;
	// commands_["QUIT"] = new Command::Quit;
	// commands_["TOPIC"] = new Command::Topic;
	// commands_["USER"] = new Command::User;
}

CommandParser::~CommandParser(void)
{
	std::map<std::string, Command *>::iterator it(commands_.begin());
	for (; it != commands_.end(); ++it)
		delete it->second;
}

void CommandParser::Trim(std::string &str)
{
	size_t start(str.find_first_not_of(" \t\r\n"));
	size_t end(str.find_last_not_of(" \t\r\n"));
	if (start == std::string::npos || end == std::string::npos)
		str.clear();
	else
		str = str.substr(start, end - start + 1);
}

bool CommandParser::ParseCommand(std::string &request, std::string &prefix,
	std::string &command)
{
	size_t	pos;

	Trim(request);
	pos = request.find(' ');
	if (request[0] == ':')
	{
		if (pos == std::string::npos)
			return (false);
		prefix = request.substr(1, pos - 1);
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

void CommandParser::ParseParams(std::string &request,
	std::vector<std::string> &params)
{
	size_t	pos;

	Trim(request);
	while (!request.empty() && params.size() < MAX_PARAMS)
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

void CommandParser::ProcessCommand(User *user, std::string &request)
{
	std::string prefix("");
	std::string command("");
	std::vector<std::string> params;
	if (!ParseCommand(request, prefix, command))
	{
		user->Write("ERROR :Prefix without command");
		return ;
	}
	if (!prefix.empty() && user->registered())
	{
		if (Server::instance->users().Search(prefix))
		{
			user->Write("ERROR :Invalid prefix \"" + prefix + "\"");
			return ;
		}
	}
	ParseParams(request, params);
	if (commands_.find(command) == commands_.end())
	{
		if (user->registered())
			user->WritePrefix(ERR_UNKNOWNCOMMAND(user->nickname(), command));
		return ;
	}
	if (!commands_[command]->UserRegistered(user))
		return ;
	if (!commands_[command]->ParamsValid(user, params.size()))
		return ;
	commands_[command]->Execute(user, params);
}
