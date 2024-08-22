/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 13:00:23 by labderra          #+#    #+#             */
/*   Updated: 2024/08/22 12:31:49 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_table
{
	int				n_philo;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	int				max_meals;
	int				end_condition;
	pthread_t		*ph_thread;
	int				*ph_meals;
	unsigned int	*lastmeal;
	pthread_mutex_t	*fork;
	unsigned int	epoch;
	pthread_mutex_t	printer;
}	t_table;

typedef struct s_init_philo
{
	int			philo_id;
	t_table		*table;
}	t_init_philo;

int				ft_atoi(const char *str);
unsigned int	my_time(void);
void			mlsleep(unsigned int w_time);
void			free_all(t_table *table);
void			info(t_table *table, int philo, char *text);
t_table			*init_table(int argc, char **argv);
void			init_philo(t_init_philo *init_ph, t_table *table);
void			*ph_routine(void *arg);

#endif
