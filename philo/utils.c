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

void	clean (t_table *table)
{
	t_philo *philo;
	int	i;

	i = -1;
	while (++i < table->philo_nbr)
	{
		philo = table->philos + i;
		handle_mutex(&philo->mutex, DESTROY);
	}
	handle_mutex(&table->write_mutex, DESTROY);
	handle_mutex(&table->mutex, DESTROY);
	free(table->forks);
	free(table->philos);
}

long	gettime(t_time time)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		error("gettimeofday failed.");
	if (time == SECOND)
		return (tv.tv_sec + (tv.tv_usec / 1e6));
	else if (time == MILLISECOND)
		return ((tv.tv_sec * 1e3) + (tv.tv_usec / 1e3));
	else if (time == MICROSECOND)
		return ((tv.tv_sec * 1e6) + tv.tv_usec);
	return (1337);
}

void	precise_usleep(long usec, t_table *table)
{
	long	start;
	long	elapsed;
	long	remaining;

	start = gettime(MICROSECOND);
	while (gettime(MICROSECOND) - start < usec)
	{
		if (sim_fin(table))
			break ;
		elapsed = gettime(MICROSECOND) - start;
		remaining = usec - elapsed;
		if (remaining > 1e3)
			usleep(remaining / 2);
		else
		{
			while (gettime(MICROSECOND) - start < usec)
				;
		}
	}
}
