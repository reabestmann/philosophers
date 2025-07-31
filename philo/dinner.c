/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbestman <rbestman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 14:32:50 by rbestman          #+#    #+#             */
/*   Updated: 2025/07/29 16:39:27 by rbestman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* thread function for the only philo thread to run
	-> simulates picking up fork and dying after time_to_die */
void	*lonely_philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	wait_threads(philo->table);
	set_long(&philo->mutex, &philo->last_meal_time, gettime(MILLISECOND));
	increase_long(&philo->table->mutex, &philo->table->threads_running);
	write_status(RIGHT_FORK, philo);
	while (!sim_fin(philo->table))
		usleep(philo->table->time_to_die);
	write_status(DIE, philo);
	return (NULL);
}

/* Runs the life cycle of a philosopher thread:
	eat, sleep, think until full or somebody died */
void	*dinner_sim(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	wait_threads(philo->table);
	set_long(&philo->mutex, &philo->last_meal_time, gettime(MILLISECOND));
	increase_long(&philo->table->mutex, &philo->table->threads_running);
	desync_philos(philo);
	while (!sim_fin(philo->table))
	{
		if (philo->full)
			break ;
		eat(philo);
		write_status(SLEEP, philo);
		precise_usleep(philo->table->time_to_sleep, philo->table);
		think(philo, false);
	}
	return (NULL);
}

/* Makes a philo think for a calculated time
		simulating delay between actions */
void	think(t_philo *philo, bool pre_sim)
{
	long	t_eat;
	long	t_sleep;
	long	t_think;

	if (!pre_sim)
		write_status(THINK, philo);
	if (philo->table->philo_nbr % 2 == 0)
		return ;
	t_eat = philo->table->time_to_eat;
	t_sleep = philo->table->time_to_sleep;
	t_think = t_eat * 2 - t_sleep;
	if (t_think < 0)
		t_think = 0;
	precise_usleep(t_think * 0.42, philo->table);
}

/* Handles fork locking, meal tracking, and timing for a philo to eat,
	releases forks after usage */
void	eat(t_philo *philo)
{
	handle_mutex(&philo->right_fork->mutex, LOCK);
	write_status(RIGHT_FORK, philo);
	handle_mutex(&philo->left_fork->mutex, LOCK);
	write_status(LEFT_FORK, philo);
	set_long(&philo->mutex, &philo->last_meal_time, gettime(MILLISECOND));
	philo->meals_eaten++;
	write_status(EAT, philo);
	precise_usleep(philo->table->time_to_eat, philo->table);
	if (philo->table->meals_nbr > 0
		&& philo->meals_eaten == philo->table->meals_nbr)
	{
		set_bool(&philo->mutex, &philo->full, true);
		write_status(FULL, philo);
	}
	handle_mutex(&philo->right_fork->mutex, UNLOCK);
	handle_mutex(&philo->left_fork->mutex, UNLOCK);
}

/* helper function to create all philo threads of table */
void	create_philos(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philo_nbr)
	{
		handle_threads(&table->philos[i].thread_id, dinner_sim,
			&table->philos[i], CREATE);
		i++;
	}
}
