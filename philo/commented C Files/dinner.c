/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbestman <rbestman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 14:32:50 by rbestman          #+#    #+#             */
/*   Updated: 2025/07/29 16:39:27 by rbestman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* thread function for the only philo thread to run
	-> simulates picking up fork and dying after time_to_die */
static void	*lonely_philo(void *arg) //void *arg is &table->philos[0]
{
	t_philo	*philo;

// 1. cast void* arg to convert it back to usable philo struct
	philo = (t_philo *)arg;
// 2. wait until all_threads_ready = true
	wait_threads(philo->table);
// 3. setting last_meal_time to now, simulation starts with a full belly
	set_long(&philo->mutex, &philo->last_meal_time, gettime(MILLISECOND));
// 4. increase number of threads currently running for monitor thread to see	
	increase_long(&philo->table->mutex, &philo->table->threads_running);
// 5. simulate philo to pick up the only fork :,(	
	write_status(RIGHT_FORK, philo);
// 6. wait until he dies :(
	while (!sim_fin(philo->table))
		usleep(philo->table->time_to_die);
// 7. When the simulation has ended, announce the philo's death	
	write_status(DIE, philo);
// 8. End the thread function by returning NULL (standard for pthreads)
	return(NULL);
}

/* Runs the life cycle of a philosopher thread:
	eat, sleep, think until full or somebody died */
static void	*dinner_sim(void *arg) // arg is &table->philo[i]
{
	t_philo *philo;
// 1. converting void * back to a philo pointer for the thread to use
	philo = (t_philo *)arg;
// 2. wait until all_threads_ready == true
	wait_threads(philo->table);
// 3. set time of last meal to now, full bellies for a fair start
	set_long(&philo->mutex, &philo->last_meal_time, gettime(MILLISECOND));
// 4. let monitor know thread is running
	increase_long(&philo->table->mutex, &philo->table->threads_running);
// 5. desync one before starting so that no deadloks happen
	desync_philos(philo);
// 6. run simulation loop until its over
	while (!sim_fin(philo->table))
	{
		if (philo->full)
			break ; // a. stop if full
		eat(philo); // b. take forks, set time, update meals, write eat status
		write_status(SLEEP, philo); // c. write sleep status
		precise_usleep(philo->table->time_to_sleep, philo->table); // d. sleep
		think(philo, false); // e. write think status and think if there is time!
	}
// 7. exit with return NULL (standard for pthreads)	
	return (NULL);
}

/* Makes a philo think for a calculated time
		simulating delay between actions */
void	think(t_philo *philo, bool pre_sim)
{
	long	t_eat;
	long	t_sleep;
	long	t_think;

	// we only announce the philo thinking if the simulation started
	if (!pre_sim)
		write_status(THINK, philo);
	// if philo_nbr is even, we dont need thinking delays
	if (philo->table->philo_nbr % 2 == 0)
		return ;
	t_eat = philo->table->time_to_eat;
	t_sleep = philo->table->time_to_sleep;
	// calculate how long the philosopher should think
	t_think = t_eat * 2 - t_sleep;
	if (t_think < 0)
		t_think = 0; // negative thinking time = no thinking time
	// think for 42% of thinking time, leave the rest for other actions
	precise_usleep(t_think * 0.42, philo->table);	
}

/* Handles fork locking, meal tracking, and timing for a philo to eat,
	releases forks after usage */
static void	eat(t_philo *philo)
{
	handle_mutex(&philo->right_fork->mutex, LOCK); // lock mutex
	write_status(RIGHT_FORK, philo); // announce philo picking up right fork
	handle_mutex(&philo->left_fork->mutex, LOCK); // lock mutex
	write_status(LEFT_FORK, philo); // announce philo picking up left fork
	// set last_meal_time to current time
	set_long(&philo->mutex, &philo->last_meal_time, gettime(MILLISECOND)); 
	philo->meals_eaten++; // increase meal counter
	write_status(EAT, philo); // announce philo eating
	precise_usleep(philo->table->time_to_eat, philo->table); // eat
	if (philo->table->max_meals > 0 && philo->meals_eaten == philo->table->max_meals)
	{
		set_bool(&philo->mutex, &philo->full, true);
		write_status(FULL,philo); // announce that philo is full
	}
	handle_mutex(&philo->right_fork->mutex, UNLOCK); // set free right fork
	handle_mutex(&philo->left_fork->mutex, UNLOCK); // set free left fork
}

/* starts all philo and monitor threads & waits for them to finish */
void	start_dinner(t_table *table)
{
	int	i;
	i = 0
	// if no meals are to be eaten, skip everything
	if (table->meals_nbr == 0)
		return ; 
	// edge case: only 1 philo
	else if (table->philo_nbr == 1)
	{
		handle_threads(&table->philos[0].thread_id, lonely_philo, &table->philos[0], CREATE);
	}
	// create philo_nbr of threads
	else
	{
		while (i < table->philo_nbr)
		{
			handle_threads(&table->philos[i].thread_id, dinner_sim, &table->philos[i], CREATE);
			i++;
		}
	}
	// create monitor thread to check if philos are dead or full
	handle_threads(&table->monitor, monitor_dinner, table, CREATE);
	// store simulation start time
	table->start_simulation = gettime(MILLISECOND);
	// signal that threads may begin
	set_bool(&table->mutex, &table->all_threads_ready, true);
	i = 0;
	while (i < table->philo_nbr) // wait for philo threads to finish
		handle_threads(&table->philos[i++].thread_id, NULL, NULL, JOIN);
	set_bool(&table->mutex, &table->end_simulation, true); // all joined? sim finished.
	handle_threads(&table->monitor, NULL, NULL, JOIN); // join monitor thread
}
