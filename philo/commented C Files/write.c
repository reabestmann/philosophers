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

void	write_status(t_status status, t_philo *philo)
{
	long	timestamp;

	// Lock the table to check if the simulation has ended
	pthread_mutex_lock(&philo->table->mutex);
	if (philo->table->end_simulation)
	{
		pthread_mutex_unlock(&philo->table->mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->table->mutex);

	// Lock the writing mutex so only one message is printed at a time
	pthread_mutex_lock(&philo->table->write_mutex);
	timestamp = gettime(MILLISECOND) - philo->table->start_simulation;

	if (status == RIGHT_FORK || status == LEFT_FORK)
		printf("%-5ld ms: %d has taken a fork\n", timestamp, philo->id);
	else if (status == EAT)
		printf("%-5ld ms: %d is eating\n", timestamp, philo->id);
	else if (status == SLEEP)
		printf("%-5ld ms: %d is sleeping\n", timestamp, philo->id);
	else if (status == THINK)
		printf("%-5ld ms: %d is thinking\n", timestamp, philo->id);
	else if (status == DIE)
	{
		// Lock again so no other philosopher prints after a death
		pthread_mutex_lock(&philo->table->mutex);
		if (!philo->table->end_simulation)
			philo->table->end_simulation = true;
		pthread_mutex_unlock(&philo->table->mutex);

		printf("%-5ld ms: %d has died\n", timestamp, philo->id);
	}
	else if (status == FULL)
		printf("%-5ld ms: %d is full\n", timestamp, philo->id);

	pthread_mutex_unlock(&philo->table->write_mutex);
}
