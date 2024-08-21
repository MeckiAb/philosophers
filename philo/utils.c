/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 11:33:44 by labderra          #+#    #+#             */
/*   Updated: 2024/08/21 13:43:05 by labderra         ###   ########.fr       */
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

unsigned int	my_time(struct timeval *clk)
{
	gettimeofday(clk, NULL);
	return (clk->tv_sec * 1000 + clk->tv_usec / 1000);
}

void	mlsleep(unsigned int w_time)
{
	unsigned int	i;

	i = 10 * w_time;
	while (i--)
		usleep(100);
}

void	free_all(t_table *table)
{
	if (table)
	{
		free(table->ph_thread);
		free(table->ph_meals);
		free(table->lastmeal);
		free(table->fork);
		free(table->clk);
		free(table);
	}
}

void	info(t_table *table, int philo, char *text)
{
	if (table->end_condition)
		return ;
	pthread_mutex_lock(&table->printer);
	printf("%5d %5d%s\n", my_time(table->clk) - table->epoch, philo, text);
	pthread_mutex_unlock(&(table->printer));
}
