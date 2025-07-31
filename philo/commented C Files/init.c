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

/* function that fills the fork structs */
static void	assign_forks(t_philo *philo, t_fork *forks, int pos)
{
	int	nbr;

	nbr = philo->table->philo_nbr;
	// wraps back to 1st fork if last philo
	philo->right_fork = &forks[(pos + 1) % nbr];
	philo->left_fork = &forks[pos];
	// if ID is even...
	if (philo->id % 2 == 0)
	{
		philo->right_fork = &forks[pos]; //opposite assignment
		philo->left_fork = &forks[(pos + 1) % nbr];
	}
	// to prevent deadlock-> make philos grap 'other' fork first
}

/* function that fills the philo struct with data */
static void	philo_init(t_table *table)
{
	int		i;
	t_philo	*philo; // to make code more readable

	i = 0;
	while (i < table->philo_nbr)
	{
		philo = table->philos + i; //assigning ptr to ptr of 'i'th philo of table
		philo->id = i + 1; //bc we dont want the first philo to be called '0'
		philo->full = false; // same as table->philos[i].full etc etc
		philo->meals_eaten = 0;
		philo->table = table; // assign table ptr so we can access table from philo struct
		handle_mutex(&philo->mutex, INIT); //bc handle_mutex expects pointer to mutex
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
