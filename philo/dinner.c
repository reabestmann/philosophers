/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbestman <rbestman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 14:32:50 by rbestman          #+#    #+#             */
/*   Updated: 2025/06/30 17:09:36 by rbestman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*dinner_sim(void *data)
{
	t_philo *philo;

	philo = (t_philo *)data;
	wait_threads(); //todo
	
	return (NULL);
}

void	start_dinner(t_table *table)
{
	int	i;

	i = -1;
	if (table->max_meals == 0)
		return ;
	else if (table->philo_nbr == 1)
		//todo
	else
	{
		while (++i < table->philo_nbr)
			handle_threads(&table->philos[i].id, dinner_sim, &table->philos[i], CREATE);
	}
	table->start_simulation = get_time(MILLISECOND);
	set_bool(&table->mutex, &table->all_threads_ready, true);
}
