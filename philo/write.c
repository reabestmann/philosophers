/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbestman <rbestman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 16:35:46 by rbestman          #+#    #+#             */
/*   Updated: 2025/07/05 20:35:45 by rbestman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* helper function to announce philo actions */
static void	print_action(t_status status, t_philo *philo, long timestamp)
{
	if (status == RIGHT_FORK || status == LEFT_FORK)
		printf("%-5ld ms: %d has taken a fork\n", timestamp, philo->id);
	else if (status == EAT)
		printf("%-5ld ms: %d is eating\n", timestamp, philo->id);
	else if (status == SLEEP)
		printf("%-5ld ms: %d is sleeping\n", timestamp, philo->id);
	else if (status == THINK)
		printf("%-5ld ms: %d is thinking\n", timestamp, philo->id);
	else if (status == FULL)
		printf("%-5ld ms: %d is full\n", timestamp, philo->id);
}

/* Helper function to announce philo death */
static void	handle_death(t_philo *philo, long timestamp)
{
	pthread_mutex_lock(&philo->table->mutex);
	if (!philo->table->end_simulation)
		philo->table->end_simulation = true;
	pthread_mutex_unlock(&philo->table->mutex);
	printf("%-5ld ms: %d has died\n", timestamp, philo->id);
}

/* function that writes status updates to stdout */
void	write_status(t_status status, t_philo *philo)
{
	long	timestamp;

	pthread_mutex_lock(&philo->table->mutex);
	if (philo->table->end_simulation)
	{
		pthread_mutex_unlock(&philo->table->mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->table->mutex);
	pthread_mutex_lock(&philo->table->write_mutex);
	timestamp = gettime(MILLISECOND) - philo->table->start_simulation;
	if (status == DIE)
		handle_death(philo, timestamp);
	else
		print_action(status, philo, timestamp);
	pthread_mutex_unlock(&philo->table->write_mutex);
}
