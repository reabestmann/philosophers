/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbestman <rbestman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 14:32:50 by rbestman          #+#    #+#             */
/*   Updated: 2025/07/05 19:28:44 by rbestman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*lonely_philo(void *arg)
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
	return(NULL);
}

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

static void	eat(t_philo *philo)
{
	handle_mutex(&philo->right_fork->fork, LOCK);
	write_status(RIGHT_FORK, philo);
	handle_mutex(&philo->left_fork->fork, LOCK);
	write_status(LEFT_FORK, philo);
	set_long(&philo->mutex, &philo->last_meal_time, gettime(MILLISECOND));
	philo->meals_eaten++;
	write_status(EAT, philo);
	precise_usleep(philo->table->time_to_eat, philo->table);
	if (philo->table->max_meals > 0 && philo->meals_eaten == philo->table->max_meals)
	{
		set_bool(&philo->mutex, &philo->full, true);
		write_status(FULL,philo);
	}
	handle_mutex(&philo->right_fork->fork, UNLOCK);
	handle_mutex(&philo->left_fork->fork, UNLOCK);
}

static void	*dinner_sim(void *data)
{
	t_philo *philo;

	philo = (t_philo *)data;
	printf("Philo %d just got born!\n", philo->id); //for debugging
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

void	start_dinner(t_table *table)
{
	int	i;

	i = -1;
	if (table->max_meals == 0)
		return ;
	else if (table->philo_nbr == 1)
	{
		handle_threads(&table->philos[0].thread_id, lonely_philo, &table->philos[0], CREATE);
	}
	else
	{
		while (++i < table->philo_nbr)
			handle_threads(&table->philos[i].thread_id, dinner_sim, &table->philos[i], CREATE);
	}
	handle_threads(&table->monitor, monitor_dinner, table, CREATE);
	table->start_simulation = gettime(MILLISECOND);
	set_bool(&table->mutex, &table->all_threads_ready, true);
	i = -1;
	while (++i < table->philo_nbr)
		handle_threads(&table->philos[i].thread_id, NULL, NULL, JOIN);
	set_bool(&table->mutex, &table->end_simulation, true);
	handle_threads(&table->monitor, NULL, NULL, JOIN);
}
