/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbestman <rbestman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 16:35:46 by rbestman          #+#    #+#             */
/*   Updated: 2025/07/05 20:18:06 by rbestman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	write_status(t_status status, t_philo *philo)
{
	long	elapsed;

	elapsed = gettime(MILLISECOND) - philo->table->start_simulation;
	handle_mutex(&philo->table->write_mutex, LOCK);
	//if (debug)
	//	write_status_debug(status, philo, elapsed); //todo - VID 1:28:00void    precise_usleep(long usec, t_table *table)
	//else {
	if (status == FULL)
		printf("%-6ldms: %d is full\n", elapsed, philo->id);
	else if (status == EAT)
		printf("%-6ldms: %d is eating\n", elapsed, philo->id); 
	else if ((status == RIGHT_FORK || status == LEFT_FORK) && !sim_fin(philo->table))
		printf("%-6ldms: %d has taken a fork\n", elapsed, philo->id);
	else if (status == SLEEP && !sim_fin(philo->table))
		printf("%-6ldms: %d is sleeping\n", elapsed, philo->id);
	else if (status == THINK && !sim_fin(philo->table))
		printf("%-6ldms: %d is thinking\n", elapsed, philo->id);
	else if (status == DIE && !sim_fin(philo->table))
		printf("%-6ldms: %d has died\n", elapsed, philo->id);
	handle_mutex(&philo->table->write_mutex, UNLOCK);
}
