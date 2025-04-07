/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncastell <ncastell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 23:42:02 by sguzman           #+#    #+#             */
/*   Updated: 2025/04/07 17:44:09 by bautrodr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_H
# define CHANNEL_H
# include "channelmanager.h"
# include "client.h"
# include "messages.h"
# include "typedefs.h"
# include <cstddef>
# include <map>
# include <set>
# include <string>
# include <vector>

class Channel
{
  public:
	Channel(const std::string &name);
	~Channel(void);

	void AddMode(char mode);
	bool HasMode(char mode) const;
	void DelMode(char mode);
	void AddMember(Client *client);
	void RemoveMember(Client *client);
	void AddInvite(Client *client);
	void AddOperator(Client *client);
	void RemoveOperator(Client *client);
	void Write(Client *sender, const std::string &message);
	bool IsMember(Client *client) const;
	bool IsInvited(Client *client) const;
	bool IsOperator(Client *client) const;
	bool IsAllowedJoin(Client *client, const std::string &key);

	std::string modes(void) const;
	std::string name(void) const;
	std::string topic(void) const;
	std::string key(void) const;
	void set_topic(std::string topic);
	void set_key(std::string key);
	void set_max_members(size_t max_members);

  private:
	std::string name_;
	std::string topic_;
	std::string key_;
	size_t max_members_;
	std::set<char> modes_;
	std::map<std::string, Client *> members_;
	std::map<std::string, Client *> invites_;
	std::map<std::string, Client *> operators_;
};
#endif /* CHANNEL_H */
