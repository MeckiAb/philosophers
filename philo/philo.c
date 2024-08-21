/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 12:59:41 by labderra          #+#    #+#             */
/*   Updated: 2024/08/21 13:38:34 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	usage(void)
{
	printf("Usage : ./philo number_of_philosophers time_to_die ");
	printf("time_to_eat time_to_sleep \n");
	printf("\t[number_of_times_each_philosopher_must_eat]\n");
}

static int	end_condition_loop(t_table *table)
{
	int	i;
	int	finished;

	while (!table->end_condition)
	{
		i = -1;
		while (++i < table->n_philo)
		{
			if (my_time(table->clk) - table->lastmeal[i] > table->time_to_die)
			{
				info(table, i + 1, " died");
				return (table->end_condition = 1, 1);
			}
		}
		finished = 0;
		while (table->max_meals > 0 && i--)
			if (table->ph_meals[i] >= table->max_meals)
				finished += 1;
		if (finished == table->n_philo)
			return (table->end_condition = 1, 1);
		mlsleep(1);
	}
	return (0);
}

static int	check_args(int argc, char **argv)
{
	if (argc != 5 && argc != 6)
		return (-1);
	if (ft_atoi(argv[1]) < 1 || ft_atoi(argv[2]) < 0 || ft_atoi(argv[3]) < 0
		|| ft_atoi(argv[4]) < 0)
		return (-1);
	if (argc == 6 && ft_atoi(argv[5]) < 1)
		return (-1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_table			*table;
	t_init_philo	*init_ph;
	int				i;

	if (check_args(argc, argv) == -1)
		return (usage(), 1);
	table = init_table(argc, argv);
	init_ph = malloc(sizeof(t_init_philo));
	if (!table || !init_ph)
		return (free_all(table), free(init_ph), 1);
	init_philo(init_ph, table);
	end_condition_loop(table);
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
