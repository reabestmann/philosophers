/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbestman <rbestman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 20:30:09 by rbestman          #+#    #+#             */
/*   Updated: 2025/06/30 16:14:05 by rbestman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*handle_malloc(size_t bytes)
{
	void	*ret;

	ret = malloc(bytes);
	if (NULL == ret)
		error("Memory Allocation failed");
	return (ret);
}

void	*handle_mutex(t_mutex *mutex, t_opcode opcode)
{
	if (opcode == LOCK)
		mutex_error(pthead_mutex_lock(mutex), opcode);
	else if (opcode == UNLOCK)
		mutex_error(pthread_mutex_unlock(mutex), opcode);
	else if (opcode == DESTROY)
		mutex_error(pthread_mutex_destroy(mutex), opcode);
	else if (opcode == INIT)
		mutex_error(pthread_mutex_init(mutex, NULL), opcode);
	else
		error("Mutex: Operation invalid - use <LOCK> <UNLOCK> <DESTROY> <INIT>");
}

void	*handle_threads(pthread_t *thread, void *(*foo)(void *), void *data, t_opcode opcode)
{
	if (opcode == CREATE)
		thread_error(pthread_create(thread, NULL, foo, data), opcode);
	if (opcode == JOIN)
		thread_error(pthread_join(*thread, NULL), opcode);
	if (opcode == DETACH)
		thread_error(pthread_detach(*thread), opcode);
	else
		error("Threads: Operation invalid - use <CREATE> <JOIN> <DETACH>");
}
