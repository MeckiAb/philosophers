/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 12:59:41 by labderra          #+#    #+#             */
/*   Updated: 2024/08/13 17:46:35 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	usage(void)
{
	printf("Usage : ./philo number_of_philosophers time_to_die ");
	printf("time_to_eat time_to_sleep \n");
	printf("\t[number_of_times_each_philosopher_must_eat]\n");
}

unsigned long int	ft_strlen(const char *s)
{
	unsigned long int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
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

unsigned int	my_time(void)
{
	struct timeval	*tv;

	tv = malloc(sizeof(struct timeval));
	gettimeofday(tv, NULL);
	return (tv->tv_sec * 1000 + tv->tv_usec / 1000);
}

void	fine_usleep(unsigned int w_time)
{
	while (w_time--)
		usleep(1);
}

table_t	*init_table(int argc, char **argv)
{
	table_t			*table;

	table = malloc(sizeof(table_t));
	table->n_philo = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	if (!table->n_philo || !table->time_to_die || !table->time_to_eat
			|| !table->time_to_sleep)
		return (free(table), NULL);
	if (argc == 6)
		table->max_meals = ft_atoi(argv[5]);
	else
		table->max_meals = -1;
	table->end_condition = 0;
	table->philo_list = malloc(sizeof(pthread_t) * table->n_philo);
	table->fork = malloc(sizeof(pthread_mutex_t) * (table->n_philo + 1));
	if (!table->philo_list || !table->fork)
		return (free(table->philo_list), free(table->fork), free(table), NULL);
	table->epoch = my_time();
	return (table);	
}

void info(table_t *table, int philo, char *text)
{
	pthread_mutex_lock(&(table->printer));
	printf("%d %d %s\n", my_time() - table->epoch, philo, text);
	pthread_mutex_unlock(&(table->printer));
}

void	*philo_routine(void *arg)
{
	int				self;
	unsigned int	last_meal;
	table_t			*table;

	table = (table_t *) arg;
	self = table->n_philo;
	last_meal = my_time() - table->epoch;
	while (1)
	{
		if (pthread_mutex_lock(&(table->fork[self])))
		{
			if (my_time() - last_meal > table->time_to_die && !table->end_condition)
				return (info(table, self, " died"), 
					pthread_mutex_unlock(&(table->fork[self])), NULL);
			else if (table->end_condition)
				return (pthread_mutex_unlock(&(table->fork[self])), NULL);
			info(table, self + 1, " has taken a fork");
		}
		if (pthread_mutex_lock(&(table->fork[self + 1])))
		{
			if (my_time() - last_meal > table->time_to_die && !table->end_condition)
				return (info(table, self, " died"), 
					pthread_mutex_unlock(&(table->fork[self])),
					pthread_mutex_unlock(&(table->fork[self + 1])), NULL);
			else if (table->end_condition)
				return (pthread_mutex_unlock(&(table->fork[self])),
					pthread_mutex_unlock(&(table->fork[self])), NULL);
			info(table, self + 1, " has taken a fork");
		}
		last_meal = my_time() - table->epoch;
		info(table, self + 1, " is eating");
		fine_usleep(table->time_to_eat * 1000);
		pthread_mutex_unlock(&(table->fork[self]));
		pthread_mutex_unlock(&(table->fork[self + 1]));
		fine_usleep(table->time_to_sleep * 1000);
		
	}
		
}

int main (int argc, char **argv)
{
	table_t	*table;
	int		philo_count;
	int		i;
	
	if (argc != 5 && argc != 6)
		return (usage(), 1);
	table = init_table(argc, argv);

printf("%p\n", table);
	
	philo_count = table->n_philo;
	i = 0;
	while (i < philo_count)
	{
		table->n_philo = i;
		pthread_mutex_init(&(table->fork[i]), NULL);
		pthread_create(&(table->philo_list[i++]), NULL, &philo_routine, (void *)table);
	}
	table->fork[i] = table->fork[0];
	pthread_mutex_init(&(table->printer), NULL);
	while (!table->end_condition)
		fine_usleep(1000);
	i = 0;
	while (i < table->n_philo)
	{
		pthread_mutex_destroy(&(table->fork[i]));
		pthread_join(table->philo_list[i], NULL);
		fine_usleep(1000);
	}
	return (0);
}