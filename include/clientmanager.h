/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientmanager.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncastell <ncastell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 23:02:45 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/27 21:15:25 by ncastell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTMANAGER_H
# define CLIENTMANAGER_H

# include "client.h"
# include "typedefs.h"
# include <map>

# define TIMEOUT 20
# define MAX_NICK_LEN 9

class ClientManager
{
  public:
	ClientManager(void);
	~ClientManager(void);

	void AddClient(int fd);
	void CloseClient(int fd, const std::string &message);
	Client *Search(const std::string &name);
	bool IsValidNick(std::string nick);
	void Read(void);
	void Read(int fd);
	void Write(void);
	void Write(int fd);
	void CheckTimeouts(void);
	void Close(void);

  private:
	std::map<int, Client *> clients;
};

#endif /* CLIENTMANAGER_H */
