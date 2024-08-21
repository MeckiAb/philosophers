/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 13:00:23 by labderra          #+#    #+#             */
/*   Updated: 2024/08/13 17:18:31 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct		table_s
{
	int				n_philo;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	int				max_meals;
	int				end_condition;
	pthread_t		*ph_thread;
	int				*ph_meals;
	unsigned int	*ph_lastmeal;
	pthread_mutex_t	*fork;
	struct timeval	*clock;
	unsigned int	epoch;
	pthread_mutex_t printer;
}	table_t;

typedef struct	init_philo_s
{
	int			philo_id;
	table_t		*table;
}	init_philo_t;

#endif
