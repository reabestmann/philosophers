/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbestman <rbestman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 20:37:55 by rbestman          #+#    #+#             */
/*   Updated: 2025/07/05 18:04:32 by rbestman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* Functions to check for spaces & digits */
static	bool	is_space(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

static	bool	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

/* Function to check if the input is a positive integer */
static const char	*valid_input(const char *str)
{
	int			len;
	const char	*num;

	len = 0;
	while (is_space(*str))
		++str;
	if (*str == '+')
		++str;
	else if (*str == '-')
		error("Input Invalid: values must be positive.");
	if (!is_digit(*str))
		error("Input Invalid: value must be a digit");
	num = str;
	while (is_digit(*str++))
		++len;
	if (len > 10)
		error("Input Invalid: value must not be larger than INT_MAX");
	return (num);
}

/* atol to protect agains overflow */
static long	ft_atol(const char *str)
{
	long	num;

	num = 0;
	str = valid_input(str);
	while (is_digit(*str))
		num = (num * 10) + (*str++ - '0');
	if (num > INT_MAX)
		error("Input Invalid: value must not be larger than INT_MAX");
	return (num);
}

/* Function to check user input & write it into table struct,
	converts time values into microseconds */
void	parse_input(t_table *table, char **argv)
{
	table->philo_nbr = ft_atol(argv[1]);
	table->time_to_die = ft_atol(argv[2]) * 1000;
	table->time_to_eat = ft_atol(argv[3]) * 1000;
	table->time_to_sleep = ft_atol(argv[4]) * 1000;
	if (argv[5])
		table->meals_nbr = ft_atol(argv[5]);
	else
		table->meals_nbr = -1;
	if (table->time_to_die < 100000
		|| table->time_to_sleep < 100000 || table->time_to_eat < 100000)
	{
		printf("Warning: Tiny time values (<100ms) can make things messy");
		printf("— the philosophers might trip over each other\n");
	}
}
