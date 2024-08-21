/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 12:59:41 by labderra          #+#    #+#             */
/*   Updated: 2024/08/21 01:44:47 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	usage(void)
{
	printf("Usage : ./philo number_of_philosophers time_to_die ");
	printf("time_to_eat time_to_sleep \n");
	printf("\t[number_of_times_each_philosopher_must_eat]\n");
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

unsigned int	my_time(struct timeval *clock)
{
	gettimeofday(clock, NULL);
	return (clock->tv_sec * 1000 + clock->tv_usec / 1000);
}

void	mlsleep(unsigned int w_time)
{
	unsigned int	i;

	i = 1 * w_time;
	while (i--)
		usleep(1000);
}

void	free_all(table_t *table)
{
	free(table->ph_thread);
	free(table->ph_meals);
	free(table->ph_lastmeal);
	free(table->fork);
	free(table->clock);
	free(table);
}

table_t	*init_table(int argc, char **argv)
{
	table_t	*table;

	table = malloc(sizeof(table_t));
	if (!table || ft_atoi(argv[1]) < 1)
		return (free(table), NULL);
	table->n_philo = ft_atoi(argv[1]);
	table->ph_thread = malloc(sizeof(pthread_t) * table->n_philo);
	table->ph_meals = malloc(sizeof(int) * table->n_philo);
	table->ph_lastmeal = malloc(sizeof(unsigned int) * table->n_philo);
	table->fork = malloc(sizeof(pthread_mutex_t) * table->n_philo);
	table->clock = malloc(sizeof(struct timeval));
	if (!table->ph_thread || !table->ph_meals || !table->ph_lastmeal
			|| !table->fork || !table->clock)
		return (free_all(table), NULL);		
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		table->max_meals = ft_atoi(argv[5]);
	else
		table->max_meals = -1;
	table->end_condition = 0;
	table->epoch = my_time(table->clock);
	return (table);	
}

void info(table_t *table, int philo, char *text)
{
	if (table->end_condition)
		return ;
	pthread_mutex_lock(&table->printer);
	printf("%d %d %s\n", my_time(table->clock) - table->epoch, philo, text);
	pthread_mutex_unlock(&(table->printer));
}

void	*ph_routine(void *arg)
{
	int		self;
	table_t	*table;

	self = ((init_philo_t *) arg)->philo_id;
	table = ((init_philo_t *) arg)->table;
	while (!table->end_condition)
	{
		info(table, self + 1, " is thinking");
		pthread_mutex_lock(&table->fork[self]);
		info(table, self + 1, " has taken a fork right");
		pthread_mutex_lock(&table->fork[(self + 1) % table->n_philo]);
		info(table, self + 1, " has taken a fork left");
		table->ph_lastmeal[self] = my_time(table->clock);
		table->ph_meals[self] += 1;
		info(table, self + 1, " is eating");
		mlsleep(table->time_to_eat);
		pthread_mutex_unlock(&table->fork[self]);
		pthread_mutex_unlock(&table->fork[(self + 1) % table->n_philo]);
		info(table, self + 1, " is sleeping");
		mlsleep(table->time_to_sleep);
	}
	return (NULL);
}

void	init_philo(init_philo_t *init_ph, table_t *table)
{
	int	i;

	i = 0;
	pthread_mutex_init(&(table->printer), NULL);
	init_ph->table = table;
	while (i < table->n_philo)
	{
		init_ph->philo_id = i;
		(table->ph_meals)[i] = 0;
		table->ph_lastmeal[i] = table->epoch;
		pthread_mutex_init(&table->fork[i], NULL);
		pthread_create(&table->ph_thread[i], 0, &ph_routine, (void *)init_ph);
		mlsleep(10);
		i++;
	}
}

int main (int argc, char **argv)
{
	table_t			*table;
	init_philo_t	*init_ph;
	int				i;
	
	if (argc != 5 && argc != 6)
		return (usage(), 1);
	table = init_table(argc, argv);
	init_ph = malloc(sizeof(init_philo_t));
	if (!table || !init_ph)
		return (free_all(table), free(init_ph), 1);
	init_philo(init_ph, table);
	while (!table->end_condition)
	{
		i = 0;
		while (i < table->n_philo)
		{
			if (my_time(table->clock) - table->ph_lastmeal[i] > table->time_to_die)
			{
				info(table, i + 1, " died");
				table->end_condition = 1;
				break ;
			}
			if (table->ph_meals[i] == table->max_meals)
			{
				table->end_condition = 1;
				break ;
			}
			i++;
		}
		mlsleep(1);
	}
	i = 0;
	while (i < table->n_philo)
	{
		pthread_mutex_destroy(&table->fork[i]);
		pthread_join(table->ph_thread[i], NULL);
		i++;
	}
	pthread_mutex_destroy(&table->printer);
	free_all(table);
	free(init_ph);
	return (0);
}
