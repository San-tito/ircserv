/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 15:33:01 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/23 16:05:22 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

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

#endif /* PARSER_HPP */
