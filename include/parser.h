/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 15:33:01 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/23 16:14:24 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "command.h"
# include "user.h"
# include <map>

class CommandParser
{
  public:
	CommandParser(void);
	~CommandParser(void);

	void ProcessCommand(User *user, std::string const &command);

  private:
	std::map<std::string, Command *> commands_;
};

#endif /* PARSER_H */
