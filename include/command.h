/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 15:33:01 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/23 16:04:25 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_H
# define COMMAND_H

# include "user.h"
# include <string>
# include <vector>

class Command
{
  public:
	Command(std::string name, int min, int max);
	virtual ~Command(void);

	bool isUserRegistered(User *user);
	bool isParamsValid(int size);

	virtual void execute(User *user,
			const std::vector<std::string> &params) = 0;

	class Invite;

  private:
	std::string name_;
	int min_;
	int max_;
};

class Command::Invite : public Command
{
  public:
	Invite(void);

	void execute(User *user, const std::vector<std::string> &params);
};

#endif /* COMMAND_H */
