/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbestman <rbestman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 15:05:28 by rbestman          #+#    #+#             */
/*   Updated: 2025/06/30 15:48:31 by rbestman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* Functions that lock the mutex before touching the variable.
	A mutex is like a toilet stall door - locked from the inside */

long	get_long(t_mutex *mutex, long *src)
{
	long	value;

	handle_mutex(mutex, LOCK);
	value = *src;
	handle_mutex(mutex, UNLOCK);
	return (value);
}

void	set_long(t_mutex *mutex, long *dest, long value)
{
	handle_mutex(mutex, LOCK);
	*dest = value;
	handle_mutex(mutex, UNLOCK);
}

bool	get_bool(t_mutex *mutex, bool *src)
{
	bool	value;

	handle_mutex(mutex, LOCK);
	value = *src;
	handle_mutex(mutex, UNLOCK);
	return (value);
}

void	set_bool(t_mutex *mutex, bool *dest, bool value)
{
	handle_mutex(mutex, LOCK);
	*dest = value;
	handle_mutex(mutex, UNLOCK);
}

bool	simulation_finished(t_table *table)
{
	return (get_bool(&table->mutex, &table->end_simulation));
}
