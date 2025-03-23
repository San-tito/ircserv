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

Event::New::New(int socket) : socket_(socket)
{
}

Event::New::~New(void)
{
}

void Event::New::Callback(short revents)
{
	static_cast<void>(revents);
	Log() << "Accepting new connection ...";
	int new_socket(accept(this->socket_, 0, 0));
	if (new_socket < 0)
	{
		Log() << "Can't accept connection: " << strerror(errno) << '!';
		Server::instance->Exit(EXIT_FAILURE);
	}
	if (!Listener::InitSocket(new_socket))
		return ;
	Server::instance->users().AddUser(new_socket);
	Log() << "Accepted new connection " << new_socket << '!';
}

Event::Client::Client(int socket) : socket_(socket)
{
}

Event::Client::~Client(void)
{
}

void Event::Client::Callback(short revents)
{
	if (revents & POLLIN)
		Server::instance->users().Read(this->socket_);
	if (revents & POLLOUT)
		Server::instance->users().Write(this->socket_);
}
