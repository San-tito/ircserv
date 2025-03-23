#ifndef EVENT_H
# define EVENT_H

# include <poll.h>
# include <string>

class Event
{
  public:
	virtual ~Event(void);
	virtual void Callback(short revents) = 0;

	class New;
	class Client;
};

class Event::New : public Event
{
  public:
	New(int socket);
	virtual ~New(void);
	virtual void Callback(short revents);

  private:
	int socket_;
};

class Event::Client : public Event
{
  public:
	Client(int socket);
	virtual ~Client(void);
	virtual void Callback(short revents);

  private:
	int socket_;
};

#endif /* EVENT_H */
