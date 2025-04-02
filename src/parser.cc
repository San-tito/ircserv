/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncastell <ncastell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 15:38:10 by sguzman           #+#    #+#             */
/*   Updated: 2025/04/03 01:30:23 by tuta             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "server.h"

CommandParser::CommandParser(void)
{
	commands_["INVITE"] = new Invite;
	commands_["JOIN"] = new Join;
	commands_["KICK"] = new Kick;
	commands_["MODE"] = new Mode;
	commands_["NICK"] = new Nick;
	commands_["PART"] = new Part;
	commands_["PASS"] = new Pass;
	commands_["PRIVMSG"] = new PrivMsg;
	commands_["QUIT"] = new Quit;
	commands_["TOPIC"] = new Topic;
	commands_["USER"] = new User;
}

CommandParser::~CommandParser(void)
{
	std::map<std::string, Command *>::iterator it(commands_.begin());
	for (; it != commands_.end(); ++it)
		delete it->second;
}

bool CommandParser::ParseCommand(std::string &request, std::string &prefix,
	std::string &command)
{
	size_t	pos;

	Tool::Trim(request);
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

	Tool::Trim(request);
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
			Tool::Trim(request);
		}
		else
		{
			params.push_back(request);
			break ;
		}
	}
}

void CommandParser::ProcessCommand(Client *client, std::string &request)
{
	std::string prefix("");
	std::string command("");
	std::vector<std::string> params;
	if (!ParseCommand(request, prefix, command))
	{
		client->Write("ERROR :Prefix without command.");
		return ;
	}
	if (!prefix.empty() && client->registered())
	{
		if (Server::instance->clients().Search(prefix))
		{
			client->Write("ERROR :Invalid prefix \"" + prefix + "\"");
			return ;
		}
	}
	ParseParams(request, params);
	if (commands_.find(command) == commands_.end())
	{
		if (client->registered() && client->nickname() != "whisper") // tremenda hardcodeadaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa REVISAR !!!!!!!
			client->WritePrefix(ERR_UNKNOWNCOMMAND(client->nickname(),
					command));
		return ;
	}
	if (!commands_[command]->ClientRegistered(client))
		return ;
	if (!commands_[command]->ParamsValid(client, params.size()))
		return ;
	commands_[command]->Execute(client, params);
}
