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

	// If philo is full, he's alive
	if (get_bool(&philo->mutex, &philo->full))
		return (false);
	// Calculate how much time passed since last meal
	since_last_meal = gettime(MILLISECOND)
		- get_long(&philo->mutex, &philo->last_meal_time);
	// Convert to milliseconds
	ms_to_die = philo->table->time_to_die / 1000;
	// if time_to_die has passed without meals, philo died :(
	if (since_last_meal > ms_to_die)
		return (true);
	// else he's alive :)
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
	// 1. Convert back void * into struct ptr using a cast
	table = (t_table *)arg;
	// 2. Wait until all threads are fully running
	while (!all_threads_running(&table->mutex,
			&table->threads_running, table->philo_nbr))
		usleep(500);
	// 3. Keep monitoring until the simulation ends
	while (!sim_fin(table))
	{
		i = 0;
		// a. loop through each philo while the simulation is running
		while (i < table->philo_nbr && !sim_fin(table))
		{
			// b. if philo died, announce death & signal end of simulation
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
	// 4. End the thread function by returning NULL
	return (NULL);
}
