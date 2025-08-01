/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbestman <rbestman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 15:42:49 by rbestman          #+#    #+#             */
/*   Updated: 2025/07/29 16:30:49 by rbestman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* starts all threads & waits for them to finish */
void	start_dinner(t_table *table)
{
	int	i;

	i = 0;
	if (table->meals_nbr == 0)
		return ;
	else if (table->philo_nbr == 1)
	{
		handle_threads(&table->philos[0].thread_id,
			lonely_philo, &table->philos[0], CREATE);
	}
	else
		create_philos(table);
	handle_threads(&table->monitor, monitor_dinner, table, CREATE);
	table->start_simulation = gettime(MILLISECOND);
	set_bool(&table->mutex, &table->all_threads_ready, true);
	while (i < table->philo_nbr)
		handle_threads(&table->philos[i++].thread_id, NULL, NULL, JOIN);
	set_bool(&table->mutex, &table->end_simulation, true);
	handle_threads(&table->monitor, NULL, NULL, JOIN);
}

/* function that fills the fork structs */
static void	assign_forks(t_philo *philo, t_fork *forks, int pos)
{
	int	nbr;

	nbr = philo->table->philo_nbr;
	philo->right_fork = &forks[(pos + 1) % nbr];
	philo->left_fork = &forks[pos];
	if (philo->id % 2 == 0)
	{
		philo->right_fork = &forks[pos];
		philo->left_fork = &forks[(pos + 1) % nbr];
	}
}

/* function that fills the philo struct with data */
static void	philo_init(t_table *table)
{
	int		i;
	t_philo	*philo;

	i = 0;
	while (i < table->philo_nbr)
	{
		philo = table->philos + i;
		philo->id = i + 1;
		philo->full = false;
		philo->meals_eaten = 0;
		philo->table = table;
		handle_mutex(&philo->mutex, INIT);
		assign_forks(philo, table->forks, i);
		i++;
	}
}

/* function that fills the table struct with data
	before starting the simulation*/
void	data_init(t_table *table)
{
	int	i;

	i = 0;
	table->end_simulation = false;
	table->all_threads_ready = false;
	table->threads_running = 0;
	table->philos = handle_malloc(sizeof(t_philo) * table->philo_nbr);
	handle_mutex(&table->mutex, INIT);
	handle_mutex(&table->write_mutex, INIT);
	table->forks = handle_malloc(sizeof(t_fork) * table->philo_nbr);
	while (i < table->philo_nbr)
	{
		handle_mutex(&table->forks[i].mutex, INIT);
		table->forks[i].id = i;
		i++;
	}
	philo_init(table);
}
