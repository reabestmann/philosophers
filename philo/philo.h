/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbestman <rbestman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 19:54:22 by rbestman          #+#    #+#             */
/*   Updated: 2025/07/05 18:54:39 by rbestman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdbool.h>
# include <limits.h>
# include <errno.h>

typedef	pthread_mutex_t t_mutex;
typedef struct s_table t_table;

typedef enum	e_opcode
{
	LOCK,
	UNLOCK,
	INIT,
	DESTROY,
	CREATE,
	JOIN,
	DETACH
}	t_opcode;

typedef	enum	e_time
{
	SECOND,
	MILLISECOND,
	MICROSECOND
}	t_time;

typedef enum	e_status
{
	EAT,
	SLEEP,
	THINK,
	RIGHT_FORK,
	LEFT_FORK,
	DIE,
	FULL
}	t_status;

typedef	struct s_fork
{
	t_mutex	fork;
	int	id;
}	t_fork;

typedef struct s_philo
{
	int	id;
	long	meals_eaten;
	bool	full;
	long	last_meal_time; // time passed from last meal
	t_fork	*left_fork;
	t_fork	*right_fork;
	pthread_t	thread_id; // a philo is a thread
	t_table	*table; // pointer to table data
	t_mutex	mutex;
}	t_philo;

typedef struct	s_table
{
	long	philo_nbr;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	long	max_meals; // [5] | FLAG if -1
	long	start_simulation;
	bool	end_simulation; // when a philo dies or all are full
	bool	all_threads_ready;
	long	threads_running;
	pthread_t	monitor;
	t_mutex	mutex;
	t_mutex	write_mutex;
	t_fork	*forks; // pointer to all forks
	t_philo	*philos; // pointer to all philos
}	t_table;

/* wrapper.c */

void	*handle_malloc(size_t bytes);
void	handle_mutex(t_mutex *mutex, t_opcode opcode);
void	handle_threads(pthread_t *thread, void *(*foo)(void *), void *data, t_opcode opcode);

/* error.c */

void	error(const char *msg);
void	thread_error(int status, t_opcode opcode);
void	mutex_error(int status, t_opcode opcode);

/* parse.c */

void    parse_input(t_table *table, char **argv);

/* init.c */

void	data_init(t_table *table);

/* dinner.c */

void	start_dinner(t_table *table);
void	think(t_philo *philo, bool pre_sim);
/* utils.c */

void	clean(t_table *table);
long    gettime(t_time time);
void    precise_usleep(long usec, t_table *table);

/* set_get.c */

void	set_bool(t_mutex *mutex, bool *dst, bool value);
void	set_long(t_mutex *mutex, long *dst, long value);
long	get_long(t_mutex *mutex, long *src);
bool	get_bool(t_mutex *mutex, bool *src);
bool	end_simulation(t_table *table);

/* sync.c */

void	wait_threads(t_table *table);
bool    all_threads_running(t_mutex *mutex, long *threads, long philo_nbr);
void    increase_long(t_mutex *mutex, long *value);
void    desync_philos(t_philo *philo);

/* write.c */

void    write_status(t_status status, t_philo *philo);

/* monitor.c */

void    *monitor_dinner(void *data);
bool	sim_fin(t_table *table);

#endif
