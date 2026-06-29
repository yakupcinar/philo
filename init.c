/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycinarog <ycinarog@student.42istanbul.com  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 01:05:18 by ycinarog          #+#    #+#             */
/*   Updated: 2026/06/29 18:26:16 by ycinarog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	assign_forks(t_philo *philo, t_data *data, int i)
{
	int	left_fork_index;
	int	right_fork_index;

	philo->id = i + 1;
	left_fork_index = i;
	right_fork_index = (i + 1) % data->philo_count;
	if (left_fork_index < right_fork_index)
	{
		philo->first_fork = &data->forks[left_fork_index];
		philo->second_fork = &data->forks[right_fork_index];
	}
	else
	{
		philo->first_fork = &data->forks[right_fork_index];
		philo->second_fork = &data->forks[left_fork_index];
	}
}

int	init_mutexes(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->philo_count);
	if (!data->forks)
		return (0);
	i = 0;
	while (i < data->philo_count)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (0);
		i++;
	}
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&data->dead_mutex, NULL) != 0)
		return (0);
	return (1);
}

int	init_philos(t_data *data, t_philo **philos)
{
	int	i;

	*philos = malloc(sizeof(t_philo) * data->philo_count);
	if (!*philos)
		return (0);
	i = 0;
	while (i < data->philo_count)
	{
		(*philos)[i].data = data;
		(*philos)[i].eat_count = 0;
		assign_forks(&(*philos)[i], data, i);
		i++;
	}
	return (1);
}

int	init_data(t_data *data, int argc, char **argv)
{
	data->philo_count = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->max_meals = ft_atoi(argv[5]);
	else
		data->max_meals = -1;
	if (data->philo_count <= 0 || data->time_to_die <= 0
		|| data->time_to_eat <= 0 || data->time_to_sleep <= 0)
		return (0);
	data->is_dead = 0;
	return (1);
}
