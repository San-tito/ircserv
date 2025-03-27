/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncastell <ncastell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 13:46:45 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/27 14:47:26 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"
#include "logging.h"
#include "server.h"

Client::Client(int socket) : socket_(socket), registered_(false),
	last_activity_(time(0)), hostname_("unknown")
{
}

Client::~Client(void)
{
	close(this->socket_);
}

void Client::Read(void)
{
	ssize_t	len;
	char	buf[READBUFFER_LEN];

	len = read(this->socket_, buf, READBUFFER_LEN);
	if (len == 0)
	{
		Server::instance->clients().DelClient(this->socket_);
		return ;
	}
	if (len < 0)
	{
		if (errno == EAGAIN)
			return ;
		Server::instance->clients().DelClient(this->socket_);
		return ;
	}
	buf[len] = '\0';
	rbuf_ += buf;
}

void Client::Write(void)
{
	ssize_t	len;

	len = write(this->socket_, wbuf_.c_str(), wbuf_.size());
	if (len < 0)
	{
		if (errno == EAGAIN)
			return ;
		Server::instance->clients().DelClient(this->socket_);
		return ;
	}
	wbuf_.clear();
}

void Client::Write(std::string const &msg)
{
	Log() << "Connection " << this->socket_ << ": " << msg;
	wbuf_ += msg + '\n';
}

void Client::Write(std::string const &prefix, std::string const &msg)
{
	Write(":" + prefix + " " + msg);
}

void Client::WritePrefix(std::string const &msg)
{
	Write(Server::instance->servername(), msg);
}

void Client::Request(void)
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
			Server::instance->clients().DelClient(this->socket_);
			return ;
		}
		rbuf_.clear();
		Server::instance->parser().ProcessCommand(this, command);
	}
}

void Client::Login(void)
{
	std::string pass(Server::instance->password());
	if (!pass.empty() && password_ != pass)
	{
		Log() << "Connection " << this->socket_ << " rejected: Bad server password";
		return (Server::instance->clients().DelClient(socket()));
	}
	registered_ = true;
	WritePrefix(RPL_WELCOME(nickname_, mask()));
	WritePrefix(RPL_YOURHOST(nickname_, Server::instance->servername(), "4.2"));
	WritePrefix(RPL_CREATED(nickname(), Server::instance->startup_time()));
	WritePrefix(RPL_MYINFO(nickname_, Server::instance->servername(), "iklot"));
}

int Client::socket(void) const
{
	return (this->socket_);
}

time_t Client::last_activity(void) const
{
	return (this->last_activity_);
}

bool Client::registered(void) const
{
	return (this->registered_);
}

std::string Client::rbuf(void) const
{
	return (this->rbuf_);
}

std::string Client::wbuf(void) const
{
	return (this->wbuf_);
}

std::string Client::username(void) const
{
	return (this->username_);
}

void Client::set_username(std::string username)
{
	this->username_ = username;
}

std::string Client::nickname(void) const
{
	return (this->nickname_);
}

void Client::set_nickname(std::string nickname)
{
	this->nickname_ = nickname;
}

std::string Client::password(void) const
{
	return (this->password_);
}

void Client::set_password(std::string pass)
{
	this->password_ = pass;
}

std::string Client::mask(void) const
{
	return (this->nickname_ + "!" + this->username_ + "@" + this->hostname_);
}
