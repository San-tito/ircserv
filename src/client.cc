/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncastell <ncastell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 13:46:45 by sguzman           #+#    #+#             */
/*   Updated: 2025/04/07 17:17:56 by bautrodr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"
#include "logging.h"
#include "server.h"

Client::Client(int socket) : socket_(socket), closing_(false),
	registered_(false), last_activity_(time(0)), hostname_("unknown")
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
		Server::instance->clients().CloseClient(this->socket_,
			"closed by client");
		return ;
	}
	if (len < 0)
	{
		if (errno == EAGAIN)
			return ;
		Server::instance->clients().CloseClient(this->socket_, "Read error");
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
		Server::instance->clients().CloseClient(this->socket_, "Write error");
		return ;
	}
	wbuf_.clear();
}

void Client::Write(std::string const &msg)
{
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
			return (Server::instance->clients().CloseClient(this->socket_,
					"Request too long"));
		rbuf_.clear();
		Server::instance->parser().ProcessCommand(this, command);
	}
}

void Client::Login(void)
{
	std::string pass(Server::instance->password());
	if (!pass.empty() && password_ != pass)
		return (Server::instance->clients().CloseClient(this->socket_,
				"Bad server password"));
	registered_ = true;
	Log() << "Connection " << socket() << ": User \"" << mask() << "\" registered";
	WritePrefix(RPL_WELCOME(nickname_, mask()));
	WritePrefix(RPL_YOURHOST(nickname_, Server::instance->servername(), "4.2"));
	WritePrefix(RPL_CREATED(nickname(), Server::instance->startup_time()));
	WritePrefix(RPL_MYINFO(nickname_, Server::instance->servername(), "iklot"));
}

int Client::socket(void) const
{
	return (this->socket_);
}

bool Client::closing(void) const
{
	return (this->closing_);
}

void Client::set_closing(bool closing)
{
	closing_ = closing;
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

