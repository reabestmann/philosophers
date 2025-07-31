/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapper.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbestman <rbestman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 20:30:09 by rbestman          #+#    #+#             */
/*   Updated: 2025/07/05 18:15:53 by rbestman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* wrapper functions to make malloc 
	and thread/mutex function usage more readable */
void	*handle_malloc(size_t bytes)
{
	void	*ret;

	ret = malloc(bytes);
	if (NULL == ret)
		error("Memory Allocation failed");
	return (ret);
}

/* mutex & thread specific errors to help spot specific problems */
void	mutex_error(int status, t_opcode opcode)
{
	if (!status)
		return ;
	if (status == EINVAL && (opcode == LOCK || opcode == UNLOCK))
		error("Mutex: Operation invalid - check initialization");
	else if (status == EINVAL && opcode == INIT)
		error("Mutex: Attribute object invalid or uninitialized");
	else if (status == EDEADLK)
		error("Mutex: Attempted double-lock - possible deadlock");
	else if (status == EPERM)
		error("Mutex: Operation not permitted - thread doesn't own lock");
	else if (status == ENOMEM)
		error("Mutex: Not enough memory");
	else if (status == EBUSY)
		error("Mutex: Destroy not possible - still in use or locked");
}

void	handle_mutex(t_mutex *mutex, t_opcode opcode)
{
	if (opcode == LOCK)
		mutex_error(pthread_mutex_lock(mutex), opcode);
	else if (opcode == UNLOCK)
		mutex_error(pthread_mutex_unlock(mutex), opcode);
	else if (opcode == DESTROY)
		mutex_error(pthread_mutex_destroy(mutex), opcode);
	else if (opcode == INIT)
		mutex_error(pthread_mutex_init(mutex, NULL), opcode);
	else
		error("Mutex: Operation invalid- use <LOCK> <UNLOCK> <DESTROY> <INIT>");
}

void	thread_error(int status, t_opcode opcode)
{
	if (!status)
		return ;
	if (status == EAGAIN)
		error("Threads: System limit reached");
	else if (status == EPERM)
		error("Threads: Missing permission for operation");
	else if (status == EINVAL && (opcode == CREATE))
		error("Threads: Operation invalid - check attributes");
	else if (status == ESRCH)
		error("Threads: Target thread invalid");
	else if (status == EDEADLK)
		error("Threads: Thread attempted to join itself - possible deadlock");
	else if (status == EINVAL && (opcode == JOIN || opcode == DETACH))
		error("Threads: Operation invalid - check thread ID");
}

void	handle_threads(pthread_t *thread, void *(*foo)(void *),
	void *data, t_opcode opcode)
{
	if (opcode == CREATE)
		thread_error(pthread_create(thread, NULL, foo, data), opcode);
	else if (opcode == JOIN)
		thread_error(pthread_join(*thread, NULL), opcode);
	else if (opcode == DETACH)
		thread_error(pthread_detach(*thread), opcode);
	else
		error("Threads: Operation invalid - use <CREATE> <JOIN> <DETACH>");
}
