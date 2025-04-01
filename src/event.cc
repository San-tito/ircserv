#include "event.h"
#include "logging.h"
#include "server.h"
#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>

Event::~Event(void)
{
}

NewConnection::NewConnection(int socket) : socket_(socket)
{
}

void NewConnection::Callback(short revents)
{
	static_cast<void>(revents);
	Log() << "Accepting new connection ...";
	int new_socket(accept(this->socket_, 0, 0));
	if (new_socket < 0)
	{
		Log() << "Can't accept connection: " << strerror(errno) << '!';
		Server::instance->Exit(EXIT_FAILURE);
	}
	if (!Tool::InitSocket(new_socket))
		return ;
	Server::instance->clients().AddClient(new_socket);
	Log() << "Connection " << new_socket << " accepted";
}

ClientSession::ClientSession(int socket) : socket_(socket)
{
}

void ClientSession::Callback(short revents)
{
	if (revents & POLLIN)
		Server::instance->clients().Read(this->socket_);
	if (revents & POLLOUT)
		Server::instance->clients().Write(this->socket_);
}
