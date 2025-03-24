/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cc                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 20:28:37 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/24 21:03:41 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bot.h"

Bot *Bot::instance(0);

Bot::Bot(void)
{
	instance = this;
}

Bot::~Bot(void)
{
}

void Bot::Exit(int status)
{
	instance = 0;
	delete (this);
	exit(status);
}

int	main(void)
{
	return (EXIT_SUCCESS);
}
