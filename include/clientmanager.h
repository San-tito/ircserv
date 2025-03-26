/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientmanager.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncastell <ncastell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 23:02:45 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/26 21:26:45 by ncastell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTMANAGER_H
# define CLIENTMANAGER_H

# include "typedefs.h"
# include "client.h"
# include <map>

# define TIMEOUT 20
# define MAX_NICK_LEN 9

class	ClientManager
{
	public:
		ClientManager(void);
		~ClientManager(void);

		void AddClient(int fd);
		void DelClient(int fd);
		Client *Search(const std::string &name);
		bool IsValidNick(std::string nick);
		void Read(void);
		void Read(int fd);
		void Write(void);
		void Write(int fd);
		void CheckTimeouts(void);

	private:
		std::map<int, Client *> clients;
};

#endif /* CLIENTMANAGER_H */
