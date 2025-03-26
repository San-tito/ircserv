/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cc                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 13:46:45 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/26 11:29:12 by tuta             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logging.h"
#include "server.h"
#include "user.h"
#include <cerrno>
#include <cstring>
#include <unistd.h>

User::User(int socket) : socket_(socket), registered_(false),
	last_activity_(time(0))
{
}

User::~User(void)
{
	close(this->socket_);
}

void User::Read(void)
{
	ssize_t	len;
	char	buf[READBUFFER_LEN];

	len = read(this->socket_, buf, READBUFFER_LEN);
	if (len == 0)
	{
		Server::instance->users().DelUser(this->socket_);
		return ;
	}
	if (len < 0)
	{
		if (errno == EAGAIN)
			return ;
		Server::instance->users().DelUser(this->socket_);
		return ;
	}
	buf[len] = '\0';
	rbuf_ += buf;
}

void User::Write(void)
{
	ssize_t	len;

	len = write(this->socket_, wbuf_.c_str(), wbuf_.size());
	if (len < 0)
	{
		if (errno == EAGAIN)
			return ;
		Server::instance->users().DelUser(this->socket_);
		return ;
	}
	wbuf_.clear();
}

void User::Write(std::string const &msg)
{
	Log() << "Connection " << this->socket_ << ": " << msg;
	wbuf_ += msg + '\n';
}

void User::WriteErrUnknownCommand(std::string const &command)
{
	Write("421 " + (this->nickname_.empty() ? "*" : this->nickname_) + " "
		+ command + " :Unknown command");
}

void User::WriteErrNeedMoreParams(std::string const &command)
{
	Write("461 " + (this->nickname_.empty() ? "*" : this->nickname_) + " "
		+ command + " :Syntax error");
}

void User::WriteErrNotRegistered(void)
{
	Write("451 " + (this->nickname_.empty() ? "*" : this->nickname_)
		+ " :Connection not registered");
}

void User::Request(void)
{
	size_t pos(0);
	std::string command("");
	std::string request(rbuf_);
	for (int i = 0; i < MAX_COMMANDS; i++)
	{
		if ((pos = request.find("\r\n")) != std::string::npos)
		{
			command = request.substr(0, pos);
			request.erase(0, pos + 2);
		}
		else if ((pos = request.find('\n')) != std::string::npos)
		{
			command = request.substr(0, pos);
			request.erase(0, pos + 1);
		}
		else
			break ;
		if (command.size() > COMMAND_LEN)
		{
			Log() << "Connection " << this->socket_ << " request too long (max ." << COMMAND_LEN << ")";
			Server::instance->users().DelUser(this->socket_);
			return ;
		}
		rbuf_.clear();
		Server::instance->parser().ProcessCommand(this, command);
	}
}

int User::socket(void) const
{
	return (this->socket_);
}

time_t User::last_activity(void) const
{
	return (this->last_activity_);
}

bool User::registered(void) const
{
	return (this->registered_);
}

std::string User::rbuf(void) const
{
	return (this->rbuf_);
}

std::string User::wbuf(void) const
{
	return (this->wbuf_);
}

std::string User::nickname(void) const
{
	return (this->nickname_);
}

std::string User::prefix(void) const
{
  return (this->nickname_ + "!" + this->username_ + "@unknown");
}
