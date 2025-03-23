/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logging.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 21:26:52 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/23 12:33:59 by sguzman          ###   ########.fr       */
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
	~Log(void);

	template <typename T> Log &operator<<(T const &value)
	{
		buffer_ << value;
		return (*this);
	}

  private:
	time_t startup_time_;
	std::ostringstream buffer_;
};

#endif /* LOGGING_HPP */
