/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbestman <rbestman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 16:14:16 by rbestman          #+#    #+#             */
/*   Updated: 2025/07/05 17:59:55 by rbestman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* Spinlock Function to check bool status */

void	wait_threads(t_table *table)
{
	while (!get_bool(&table->mutex, &table->all_threads_ready))
		usleep(500);
}

void	desync_philos(t_philo *philo)
{
	if (philo->table->philo_nbr % 2 == 0)
	{
		if (philo->id % 2 == 0)
			precise_usleep(3e4, philo->table);
	}
	else
	{
		if (philo->id % 2)
			think(philo, true);
	}
}

void	increase_long(t_mutex *mutex, long *value)
{
	handle_mutex(mutex, LOCK);
	(*value)++;
	handle_mutex(mutex, UNLOCK);
}

bool	all_threads_running(t_mutex *mutex, long *threads, long philo_nbr)
{
	bool	ret;

	ret = false;
	handle_mutex(mutex, LOCK);
	if (*threads == philo_nbr)
		ret = true;
	handle_mutex(mutex, UNLOCK);
	return (ret);
}
