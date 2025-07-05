/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbestman <rbestman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 17:21:02 by rbestman          #+#    #+#             */
/*   Updated: 2025/07/05 20:20:09 by rbestman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool philo_died(t_philo *philo)
{
	long	elapsed;
	long	tod;

	if (get_bool(&philo->mutex, &philo->full))
		return (false);
	elapsed = gettime(MILLISECOND) - get_long(&philo->mutex, &philo->last_meal_time);
	tod = philo->table->time_to_die / 1e3;
	if (elapsed > tod)
		return (true);
	return (false);
}

bool sim_fin(t_table *table)
{
	return get_bool(&table->mutex, &table->end_simulation);
}

/*bool sim_fin(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->philo_nbr)
	{
		if (philo_died(&table->philos[i]))
			return (true);
		else if (table->philos[i].meals_eaten < table->max_meals)
			return (false);
	}
	return (true);
}*/

void *monitor_dinner(void *arg)
{
	t_table *table;
	int i;

	table = (t_table *)arg;

	// Wait until all threads are fully running
	while (!all_threads_running(&table->mutex, &table->threads_running, table->philo_nbr))
		usleep(500);

	// Keep monitoring until the simulation ends
	while (!sim_fin(table))
	{
		i = 0;
		while (i < table->philo_nbr && !sim_fin(table))
		{
			// Check if this philo died
			if (philo_died(&table->philos[i]))
			{
				write_status(DIE, &table->philos[i]);
				set_bool(&table->mutex, &table->end_simulation, true);
				break ;
			}
			i++;
		}
		usleep(100); // small sleep to reduce CPU stress
	}

	return (NULL);
}

/*void	*monitor_dinner(void *data)
{
	int	i;
	t_table *table;

	table = (t_table *)data;
	while (!all_threads_running(&table->mutex, &table->threads_running, table->philo_nbr))
		usleep(500);
	while (!sim_fin(table))
	{
		i = -1;
		while (++i < table->philo_nbr && !sim_fin(table))
		{
			if (philo_died(table->philos + i))
			{
				set_bool(&table->mutex, &table->end_simulation, true);
				write_status(DIE, table->philos + i);
			}
		}
		usleep (500);
	}
	return (NULL);
}*/
