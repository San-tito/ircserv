/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   botparser.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncastell <ncastell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 16:32:48 by ncastell          #+#    #+#             */
/*   Updated: 2025/03/28 16:36:04 by ncastell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

class BotParser
{
public:
	BotParser(void);
	~BotParser(void);

	void	Trim(std::string &str);
	bool	ParseCommand(std::string &);
	void	ParseParams(std::string &request, std::vector<std::string> &params);
	void	ProcessCommand(Client *client, std::string &request);

private:
	std::map<std::string, Command *> commands_;
};