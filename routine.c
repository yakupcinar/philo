/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycinarog <ycinarog@student.42istanbul.com  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 01:05:29 by ycinarog          #+#    #+#             */
/*   Updated: 2026/06/29 18:56:24 by ycinarog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_philo *philo, char *status)
{
	long long	time;

	pthread_mutex_lock(&philo->data->print_mutex);
	if (!check_if_dead(philo->data) || status[0] == 'd')
	{
		time = get_current_time() - philo->data->start_time;
		printf("%lld %d %s\n", time, philo->id, status);
	}
	pthread_mutex_unlock(&philo->data->print_mutex);
}

static void	*single_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(philo->first_fork);
	print_status(philo, "has taken a fork");
	ft_usleep(philo->data->time_to_die);
	pthread_mutex_unlock(philo->first_fork);
	return (NULL);
}

static void	eat_routine(t_philo *philo)
{
	pthread_mutex_lock(philo->first_fork);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(philo->second_fork);
	print_status(philo, "has taken a fork");
	print_status(philo, "is eating");
	pthread_mutex_lock(&philo->data->dead_mutex);
	philo->last_meal_time = get_current_time();
	philo->eat_count++;
	pthread_mutex_unlock(&philo->data->dead_mutex);
	ft_usleep(philo->data->time_to_eat);
	pthread_mutex_unlock(philo->second_fork);
	pthread_mutex_unlock(philo->first_fork);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->philo_count == 1)
		return (single_philo_routine(philo));
	if (philo->id % 2 == 0)
		ft_usleep(15);
	while (!check_if_dead(philo->data))
	{
		eat_routine(philo);
		print_status(philo, "is sleeping");
		ft_usleep(philo->data->time_to_sleep);
		if (check_if_dead(philo->data))
			break ;
		print_status(philo, "is thinking");
		if (philo->data->philo_count % 2 != 0)
			ft_usleep(5);
	}
	return (NULL);
}

static int	check_philos(t_data *data, t_philo *philos, int *all_finished)
{
	int	i;

	i = -1;
	while (++i < data->philo_count)
	{
		pthread_mutex_lock(&data->dead_mutex);
		if (get_current_time() - philos[i].last_meal_time >= data->time_to_die)
		{
			data->is_dead = 1;
			pthread_mutex_unlock(&data->dead_mutex);
			print_status(&philos[i], "died");
			return (1);
		}
		if (data->max_meals != -1 && philos[i].eat_count < data->max_meals)
			*all_finished = 0;
		pthread_mutex_unlock(&data->dead_mutex);
	}
	return (0);
}

void	monitor_routine(t_data *data, t_philo *philos)
{
	int	all_finished;

	while (1)
	{
		all_finished = 1;
		if (check_philos(data, philos, &all_finished))
			return ;
		if (data->max_meals != -1 && all_finished == 1)
		{
			pthread_mutex_lock(&data->dead_mutex);
			data->is_dead = 1;
			pthread_mutex_unlock(&data->dead_mutex);
			return ;
		}
		usleep(500);
	}
}
