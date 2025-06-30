/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbestman <rbestman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 20:25:23 by rbestman          #+#    #+#             */
/*   Updated: 2025/06/30 15:04:57 by rbestman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* USAGE: ./philo 5(philo_nbr) 800(time_to_die) 200(time_to_eat) 200(time_to_sleep) [5](meal_limit) */
int	main(int params, char	**argv)
{
	if (params == 5 || params == 6) //correct input
	{
		parse_input(&table, argv);
		data_init(&table);
		start_dinner(&table);
		clean(&table);
	}
	else
	{
		error("USAGE: ./philo (philo_nbr) (time_to_die) (time_to_eat) (time_to_sleep) [max_meals]");
	}
	return (0);
}
