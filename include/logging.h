/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logging.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 21:26:52 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/22 21:36:32 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGING_HPP
# define LOGGING_HPP

# include <ctime>
# include <iostream>
# include <sstream>

class Log
{
  public:
	Log(void);
	template <typename T> Log &operator<<(T const &value)
	{
		buffer << value;
		return (*this);
	}
	virtual ~Log(void);

  private:
	time_t startup_time;
	std::ostringstream buffer;
};

#endif /* LOGGING_HPP */
