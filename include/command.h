/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncastell <ncastell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 15:33:01 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/26 11:13:54 by sguzman          ###   ########.fr       */
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

	bool UserRegistered(User *user);
	bool ParamsValid(User *user, int size);

	virtual void Execute(User *user,
			const std::vector<std::string> &params) = 0;

  private:
	std::string name_;
	int min_;
	int max_;
};

class Invite : public Command
{
  public:
	Invite(void);

	void Execute(User *user, const std::vector<std::string> &params);
};

class Join : public Command
{
  public:
	Join(void);

	void Execute(User *user, const std::vector<std::string> &params);

  private:
	// bool IsAllowed(User *user, Channel chan, std::string key); -- TODO --
};

#endif /* COMMAND_H */
