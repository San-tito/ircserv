/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 20:28:00 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/22 20:54:39 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

# include <algorithm>
# include <cfloat>
# include <climits>
# include <csignal>
# include <cstdarg>
# include <cstring>
# include <iostream>
# include <map>
# include <set>
# include <sstream>
# include <string>
# include <vector>

class Bot
{
  public:
	Bot(void);
	~Bot(void);

	void Exit(int status);

	static Bot *instance;
};

#endif /* BOT_HPP */
