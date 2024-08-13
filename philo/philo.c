/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 12:59:41 by labderra          #+#    #+#             */
/*   Updated: 2024/08/13 11:16:38 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	usage(void)
{
	printf("Usage : ./philo number_of_philosophers time_to_die ");
	printf("time_to_eat time_to_sleep \n");
	printf("\t[number_of_times_each_philosopher_must_eat]\n");
}

char	*ft_strdup(const char *s1)
{
	char	*p;
	size_t	i;

	p = (char *)malloc(sizeof(char) * (ft_strlen(s1) + 1));
	if (!p)
		return (NULL);
	i = 0;
	while (i < ft_strlen(s1) + 1)
	{
		p[i] = s1[i];
		i++;
	}
	return (p);
}

char	*ft_itoa(int n)
{
	char	str[15];
	int		i;
	int		sign;

	sign = 0;
	i = 14;
	str[i--] = '\0';
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	else if (n < 0)
	{
		n = -n;
		sign = -1;
	}
	else if (n == 0)
		return (ft_strdup("0"));
	while (n > 0)
	{
		str[i] = n % 10 + '0';
		n = n / 10;
		i--;
	}
	if (sign == -1)
		str[i--] = '-';
	return (ft_strdup(&str[i + 1]));
}

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
	return (sign * acc);
}

table_t	*init_table(int argc, char **argv)
{
	table_t	*table;

	table = malloc(sizeof(table_t));
	table->n_philo = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	if (!table->n_philo || !table->time_to_die || !table->time_to_eat
			|| table->time_to_sleep)
		return (free(table), NULL);
	if (argc == 6)
		table->max_meals = ft_atoi(argv[5]);
	else
		table->max_meals = -1;
	table->end_condition = 0;
	table->philo_list = malloc(sizeof(pthread_t) * table->n_philo);
	table->fork = malloc(sizeof(pthread_mutex_t) * table->n_philo);
	if (!table->philo_list || !table->fork)
		return (free(table->philo_list), free(table->fork), free(table), NULL);
	return (table);	
}
unsigned int	my_time(void)
{
	struct timeval	*tv;
	unsigned int	timestamp;
	
	gettimeofday(tv, NULL);
	tv->tv_sec;
	
}

void	fine_usleep(unsigned int w_time)
{
	while (w_time--)
		usleep(1);
}

void	philo_routine(table_t *table)
{
	int	self;

	self = table->n_philo;
	
	
}

int main (int argc, char **argv)
{
	table_t	*table;
	int		philo_count;
	int		i;
	
	if (argc != 5 && argc != 6)
		return (usage(), 1);
	table = init_table(argc, argv);
	i = 0;
	while (i < philo_count)
	{
		table->n_philo = i;
		pthread_mutex_init(&(table->fork[i]), NULL);
		pthread_create(&(table->philo_list[i++]), NULL, &philo_routine, table);
	}
	while (!table->end_condition)
		fine_usleep(1000);
	i = 0;
	while (i < table->n_philo)
	{
		pthread_mutex_destroy(&(table->fork[i]));
		pthread_cancel(table->philo_list[i]);

	}
	return (0);
}