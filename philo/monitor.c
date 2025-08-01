/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*													+:+ +:+		 +:+	 */
/*   By: rbestman <rbestman@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/07/04 17:21:02 by rbestman		  #+#	#+#			 */
/*   Updated: 2025/07/29 16:18:04 by rbestman         ###   ########.fr       */
/*																			*/
/* ************************************************************************** */

#include "philo.h"

/* Function to check if a given philo has died */
static bool	philo_died(t_philo *philo)
{
	long	since_last_meal;
	long	ms_to_die;

	if (get_bool(&philo->mutex, &philo->full))
		return (false);
	since_last_meal = gettime(MILLISECOND)
		- get_long(&philo->mutex, &philo->last_meal_time);
	ms_to_die = philo->table->time_to_die / 1000;
	if (since_last_meal > ms_to_die)
		return (true);
	return (false);
}

/* Function that returns the simulation status with increased readability */
bool	sim_fin(t_table *table)
{
	return (get_bool(&table->mutex, &table->end_simulation));
}

/* Function run by monitor thread that checks the table for dead philos.
	Announces any detected deaths & marks end of simulation */
void	*monitor_dinner(void *arg) // arg is table
{
	t_table	*table;
	int		i;

	table = (t_table *)arg;
	while (!all_threads_running(&table->mutex,
			&table->threads_running, table->philo_nbr))
		usleep(500);
	while (!sim_fin(table))
	{
		i = 0;
		while (i < table->philo_nbr && !sim_fin(table))
		{
			if (philo_died(&table->philos[i]))
			{
				write_status(DIE, &table->philos[i]);
				set_bool(&table->mutex, &table->end_simulation, true);
				break ;
			}
			i++;
		}
		usleep(100);
	}
	return (NULL);
}
