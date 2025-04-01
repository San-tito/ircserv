/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncastell <ncastell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 15:33:01 by sguzman           #+#    #+#             */
/*   Updated: 2025/04/01 11:35:52 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "client.h"
# include "command.h"
# include "messages.h"
# include <map>

# define MAX_PARAMS 15

class CommandParser
{
  public:
	CommandParser(void);
	~CommandParser(void);

	bool ParseCommand(std::string &request, std::string &prefix,
		std::string &command);
	void ParseParams(std::string &request, std::vector<std::string> &params);
	void ProcessCommand(Client *client, std::string &request);

  private:
	std::map<std::string, Command *> commands_;
};

#endif /* PARSER_H */
