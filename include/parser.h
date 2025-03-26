/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncastell <ncastell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 15:33:01 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/26 21:30:01 by ncastell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "command.h"
# include "messages.h"
# include "client.h"
# include <map>

# define MAX_PARAMS 15

class CommandParser
{
  public:
	CommandParser(void);
	~CommandParser(void);

	void Trim(std::string &str);
	bool ParseCommand(std::string &request, std::string &prefix,
		std::string &command);
	void ParseParams(std::string &request, std::vector<std::string> &params);
	void ProcessCommand(Client *client, std::string &request);

  private:
	std::map<std::string, Command *> commands_;
};

#endif /* PARSER_H */
