/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycinarog <ycinarog@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 10:49:34 by ycinarog          #+#    #+#             */
/*   Updated: 2026/06/30 10:59:30 by ycinarog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_data
{
	int				philo_count;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	int				max_meals;
	long long		start_time;
	int				is_dead;

	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	dead_mutex;
}					t_data;

typedef struct s_philo
{
	int				id;
	int				eat_count;
	long long		last_meal_time;

	pthread_t		thread;

	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	t_data			*data;
}					t_philo;

long long			get_current_time(void);
void				ft_usleep(int time_in_ms);
int					check_if_dead(t_data *data);
int					ft_atoi(const char *nptr);

int					init_mutexes(t_data *data);
int					init_philos(t_data *data, t_philo **philos);
int					init_data(t_data *data, int argc, char **argv);

void				*philosopher_routine(void *arg);
void				monitor_routine(t_data *data, t_philo *philos);
void				print_status(t_philo *philo, char *status);
int					check_if_dead(t_data *data);

int					start_simulation(t_data *data, t_philo *philos);

#endif