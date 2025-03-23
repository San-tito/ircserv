/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 15:33:01 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/23 16:55:23 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "command.h"
# include "user.h"
# include <map>

# define MAX_PARAMS 15

class CommandParser
{
  public:
	CommandParser(void);
	~CommandParser(void);

	void Trim(std::string &str);
	bool ParseCommand(std::string &request, std::string &command);
	void ParseParams(std::string &request, std::vector<std::string> &params);
	void ProcessCommand(User *user, std::string &request);

  private:
	std::map<std::string, Command *> commands_;
};

#endif /* PARSER_H */
