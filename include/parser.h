/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 15:33:01 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/23 15:40:21 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include "user.h"

class CommandParser
{
  public:
	CommandParser(void);
	~CommandParser(void);

	void ProcessCommand(User *user, std::string const &command);
};

#endif /* PARSER_HPP */
