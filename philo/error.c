/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbestman <rbestman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 16:14:29 by rbestman          #+#    #+#             */
/*   Updated: 2025/06/29 17:55:04 by rbestman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	error(const char *msg)
{
	printf("Error: %s\n", msg);
	exit (1);
}

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
