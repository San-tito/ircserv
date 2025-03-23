/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logging.cc                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 21:30:43 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/23 01:07:42 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logging.h"

Log::Log(void)
{
	startup_time_ = time(0);
}

Log::~Log(void)
{
	char	str[64];

	strftime(str, 64, "%H:%M:%S", localtime(&startup_time_));
	std::cout << "[" << str << "] " << buffer_.str() << '\n';
}
