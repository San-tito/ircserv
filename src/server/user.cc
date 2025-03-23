/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cc                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 13:46:45 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/23 13:48:00 by sguzman          ###   ########.fr       */
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
}

void User::Read(void)
{
	static_cast<void>(this->socket_);
}

void User::Write(void)
{
	static_cast<void>(this->socket_);
}
