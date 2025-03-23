#include "event.h"
#include "server.h"
#include "logging.h"
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
	if (revents & POLLIN)
	{
		Log() << "Accepting new connection ...";
		int new_socket = accept(this->socket_, 0, 0);
		if (new_socket < 0)
		{
			Log() << "Can't accept connection: " << strerror(errno) << '!';
			Server::instance->Exit(EXIT_FAILURE);
		}
		Server::instance->events().AddEvent<Event::Client>(new_socket, POLLIN);
	}
}

Event::Client::Client(int socket) : socket_(socket)
{
}

Event::Client::~Client(void)
{
	close(this->socket_);
}

void Event::Client::Callback(short revents)
{
	if (revents & POLLIN)
	{
		char buffer[1024];
		ssize_t bytes_read = read(this->socket_, buffer, sizeof(buffer) - 1);
		if (bytes_read <= 0)
		{
			Log() << "Client disconnected or error occurred.";
			Server::instance->events().DelEvent(this->socket_);
			delete this;
		}
		else
		{
			buffer[bytes_read] = '\0';
			Log() << "Received: " << buffer;
			// Handle client message here
		}
	}
}
