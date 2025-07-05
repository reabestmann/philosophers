/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbestman <rbestman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 15:42:49 by rbestman          #+#    #+#             */
/*   Updated: 2025/07/05 18:37:56 by rbestman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

static void philo_init(t_table *table)
{
	int	i;
	t_philo	*philo;

	i = -1;
	while (++i < table->philo_nbr)
	{
		philo = table->philos + i;
		philo->id = i + 1;
		philo->full = false;
		philo->meals_eaten = 0;
		philo->table = table;
		handle_mutex(&philo->mutex, INIT);
		assign_forks(philo, table->forks, i);
	}
}

void	data_init(t_table *table)
{
	int	i;

	i = -1;
	table->end_simulation = false;
	table->all_threads_ready = false;
	table->threads_running = 0;
	table->philos = handle_malloc(sizeof(t_philo) * table->philo_nbr);
	handle_mutex(&table->mutex, INIT);
	handle_mutex(&table->write_mutex, INIT);
	table->forks = handle_malloc(sizeof(t_fork) * table->philo_nbr);
	while (++i < table->philo_nbr)
	{
		handle_mutex(&table->forks[i].fork, INIT);
		table->forks[i].id = i; 
	}
	philo_init(table);
}
