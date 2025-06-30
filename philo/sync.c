/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbestman <rbestman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 16:14:16 by rbestman          #+#    #+#             */
/*   Updated: 2025/06/30 16:19:04 by rbestman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* Spinlock Function to check bool status */

void	wait_threads(t_table *table)
{
	while (!get_bool(&table->table_mutex, &table->all_threads_ready))
		usleep(500);
}
