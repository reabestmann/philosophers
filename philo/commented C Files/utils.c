/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbestman <rbestman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 16:20:51 by rbestman          #+#    #+#             */
/*   Updated: 2025/07/05 18:05:02 by rbestman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* Cleans up allocated memory after simulation finishes */
void	clean (t_table *table)
{
	t_philo *philo;
	int	i;

	i = 0;
	// 1. destroy all philo mutexes
	while (i < table->philo_nbr)
	{
		philo = table->philos + i;
		handle_mutex(&philo->mutex, DESTROY);
		i++;
	}
	// 2. destroy write and table mutex
	handle_mutex(&table->write_mutex, DESTROY);
	handle_mutex(&table->mutex, DESTROY);
	// 3. free memory
	free(table->forks);
	free(table->philos);
}

/* Returns the current time in specified unit */
long	gettime(t_time time)
{
	// 0. struct timeval is defined in <sys/time.h>
	struct timeval	tv;

	// 1. get current time into 'tv' (seconds + microseconds)
	if (gettimeofday(&tv, NULL))
		error("gettimeofday failed.");
	// 2. return time in seconds
	if (time == SECOND)
		return (tv.tv_sec + (tv.tv_usec / 1000000));
	// 3. return time in milliseconds
	else if (time == MILLISECOND)
		return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
	// 4. return time in microseconds
	else if (time == MICROSECOND)
		return ((tv.tv_sec * 1000000) + tv.tv_usec);
	// 5. fallback return, shouldn't be reached
	return (0);
}

/* Pauses execution for an accurate specified duration
	in milliseconds, minimizing CPU usage and time drifting*/
void	precise_usleep(long time_to_wait, t_table *table)
{
	long  target_time;
	long	time_left;

	// Save the target time in microseconds
  target_time = gettime(MICROSECOND) + time_to_wait;

	// Loop until the total desired time has passed
	while (gettime(MICROSECOND) < target_time)
	{
		// Stop early if simulation is finished
		if (sim_fin(table))
			break ;

		// How much more time do we still need to wait?
		time_left = target_time - (gettime(MICROSECOND));

		// If there's more than 1000 microseconds (1 millisecond) left
		if (time_left > 1000)
			usleep(time_left / 2); // sleep for half of the remaining time
		else
		{
			// If under 1 ms left, wait by checking the time constantly
			while (gettime(MICROSECOND) < target_time)
				; // do nothing, just wait
		}
	}
}

