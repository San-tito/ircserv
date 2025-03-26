/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncastell <ncastell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 15:33:01 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/27 00:20:00 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_H
# define COMMAND_H

# include "client.h"
# include <string>
# include <vector>

class Command
{
  public:
	Command(std::string name, int min, int max, bool register_req);
	virtual ~Command(void);

	bool ClientRegistered(Client *client);
	bool ParamsValid(Client *client, int size);

	virtual void Execute(Client *client,
			const std::vector<std::string> &params) = 0;

  protected:
	std::string name_;

  private:
	int min_;
	int max_;
	bool register_req_;
};

class Invite : public Command
{
  public:
	Invite(void);

	void Execute(Client *client, const std::vector<std::string> &params);
};

class Join : public Command
{
  public:
	Join(void);

	void Execute(Client *client, const std::vector<std::string> &params);

  private:
	// bool IsAllowed(Client *client, Channel chan, std::string key); -- TODO --
};

class Pass : public Command
{
  public:
	Pass(void);

	void Execute(Client *client, const std::vector<std::string> &params);
};

class Nick : public Command
{
  public:
	Nick(void);

	void Execute(Client *client, const std::vector<std::string> &params);
};

class User : public Command
{
  public:
	User(void);

	void Execute(Client *client, const std::vector<std::string> &params);
};

#endif /* COMMAND_H */
