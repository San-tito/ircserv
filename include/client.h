/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncastell <ncastell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 12:33:03 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/27 15:12:42 by ncastell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

# include <cerrno>
# include <cstring>
# include <ctime>
# include <string>
# include <unistd.h>

# define READBUFFER_LEN 2048
# define MAX_COMMANDS 3
# define COMMAND_LEN 512

class Client
{
	public:
		Client(int socket);
		~Client(void);

		void Read(void);
		void Write(void);
		void Write(std::string const &msg);
		void Write(std::string const &prefix, std::string const &msg);
		void WritePrefix(std::string const &msg);
		void Request(void);
		void Login(void);

		int socket(void) const;
		time_t last_activity(void) const;
		bool registered(void) const;
		std::string rbuf(void) const;
		std::string wbuf(void) const;
		std::string nickname(void) const;
		void set_nickname(std::string nickname);
		std::string username(void) const;
		void set_username(std::string Clientname);
		std::string password(void) const;
		void set_password(std::string pass);
		std::string mask(void) const;

	private:
		int socket_;
		bool registered_;
		time_t last_activity_;
		std::string rbuf_;
		std::string wbuf_;
		std::string nickname_;
		std::string username_;
		std::string password_;
		std::string hostname_;
};

#endif /* CLIENT_H */
