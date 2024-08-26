/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 11:33:44 by labderra          #+#    #+#             */
/*   Updated: 2024/08/26 10:49:08 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int	sign;
	int	acc;

	sign = 1;
	acc = 0;
	while (*str == '\n' || *str == '\t' || *str == '\v' || *str == '\f'
		|| *str == '\r' || *str == ' ')
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		acc = acc * 10 + *str - 48;
		str++;
	}
	if (!*str)
		return (sign * acc);
	else
		return (-1);
}

unsigned int	my_time(void)
{
	struct timeval	clock;

	gettimeofday(&clock, NULL);
	return (clock.tv_sec * 1000 + clock.tv_usec / 1000);
}

void	mlsleep(unsigned long int w_time)
{
	unsigned long int	now;
	unsigned long int	start;
	struct timeval		clock;

	if (w_time < 1)
		return ;
	gettimeofday(&clock, NULL);
	start = clock.tv_sec * 1000000 + clock.tv_usec;
	w_time = w_time * 1000 + start;
	now = start;
	while (now <= w_time)
	{
		if (w_time - now > 1000)
			usleep((w_time - now) / 2);
		gettimeofday(&clock, NULL);
		now = clock.tv_sec * 1000000 + clock.tv_usec;
	}
}

void	free_all(t_table *table)
{
	if (table)
	{
		free(table->ph_thread);
		free(table->ph_meals);
		free(table->lastmeal);
		free(table->fork);
		free(table);
	}
}

void	info(t_table *table, int philo, char *text)
{
	if (table->end_condition)
		return ;
	pthread_mutex_lock(&table->printer);
	printf("%5u %5d%s\n", my_time() - table->epoch, philo, text);
	pthread_mutex_unlock(&(table->printer));
}
