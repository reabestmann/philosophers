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

/* error function to print a message and exit safely*/
void	error(const char *msg)
{
	printf("Error: %s\n", msg);
	exit (1);
}

/* Cleans up allocated memory after simulation finishes */
void	clean(t_table *table)
{
	t_philo	*philo;
	int		i;

	i = 0;
	while (i < table->philo_nbr)
	{
		philo = table->philos + i;
		handle_mutex(&philo->mutex, DESTROY);
		i++;
	}
	handle_mutex(&table->write_mutex, DESTROY);
	handle_mutex(&table->mutex, DESTROY);
	free(table->forks);
	free(table->philos);
}

/* Returns the current time in specified unit */
long	gettime(t_time time)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		error("gettimeofday failed.");
	if (time == SECOND)
		return (tv.tv_sec + (tv.tv_usec / 1000000));
	else if (time == MILLISECOND)
		return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
	else if (time == MICROSECOND)
		return ((tv.tv_sec * 1000000) + tv.tv_usec);
	return (0);
}

/* Pauses execution for an accurate specified duration
	in milliseconds, minimizing CPU usage and time drifting*/
void	precise_usleep(long time_to_wait, t_table *table)
{
	long	target_time;
	long	time_left;

	target_time = gettime(MICROSECOND) + time_to_wait;
	while (gettime(MICROSECOND) < target_time)
	{
		if (sim_fin(table))
			break ;
		time_left = target_time - (gettime(MICROSECOND));
		if (time_left > 1000)
			usleep(time_left / 2);
		else
		{
			while (gettime(MICROSECOND) < target_time)
				;
		}
	}
}
