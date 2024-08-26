/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 11:35:50 by labderra          #+#    #+#             */
/*   Updated: 2024/08/26 09:46:41 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_table	*init_table(int argc, char **argv)
{
	t_table	*table;

	table = malloc(sizeof(t_table));
	if (!table || ft_atoi(argv[1]) < 1)
		return (free(table), NULL);
	table->n_philo = ft_atoi(argv[1]);
	table->ph_thread = malloc(sizeof(pthread_t) * table->n_philo);
	table->ph_meals = malloc(sizeof(int) * table->n_philo);
	table->lastmeal = malloc(sizeof(unsigned int) * table->n_philo);
	table->fork = malloc(sizeof(pthread_mutex_t) * table->n_philo);
	if (!table->ph_thread || !table->ph_meals || !table->lastmeal
		|| !table->fork)
		return (free_all(table), NULL);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		table->max_meals = ft_atoi(argv[5]);
	else
		table->max_meals = -1;
	table->end_condition = 0;
	table->epoch = my_time();
	return (table);
}

void	init_philo(t_init_philo *init_ph, t_table *table)
{
	int	i;

	i = 0;
	pthread_mutex_init(&(table->printer), NULL);
	init_ph->table = table;
	while (i < table->n_philo)
	{
		(table->ph_meals)[i] = 0;
		table->lastmeal[i] = table->epoch;
		pthread_mutex_init(&table->fork[i], NULL);
		i++;
	}
	i = 0;
	while (i < table->n_philo)
	{
		init_ph->philo_id = i;
		pthread_create(&table->ph_thread[i], 0, &ph_routine, (void *)init_ph);
		mlsleep(1);
		i++;
	}
}

void	*ph_routine(void *arg)
{
	int		self;
	t_table	*table;

	self = ((t_init_philo *) arg)->philo_id;
	table = ((t_init_philo *) arg)->table;
	while (!table->end_condition)
	{
		info(table, self + 1, " is thinking");
		pthread_mutex_lock(&table->fork[self]);
		info(table, self + 1, " has taken a fork");
		pthread_mutex_lock(&table->fork[(self + 1) % table->n_philo]);
		info(table, self + 1, " has taken a fork");
		table->lastmeal[self] = my_time();
		table->ph_meals[self] += 1;
		info(table, self + 1, " is eating");
		mlsleep(table->time_to_eat);
		pthread_mutex_unlock(&table->fork[(self + 1) % table->n_philo]);
		pthread_mutex_unlock(&table->fork[self]);
		info(table, self + 1, " is sleeping");
		mlsleep(table->time_to_sleep);
	}
	return (NULL);
}
