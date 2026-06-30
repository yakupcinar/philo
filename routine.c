/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycinarog <ycinarog@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 01:05:29 by ycinarog          #+#    #+#             */
/*   Updated: 2026/06/30 00:05:38 by ycinarog         ###   ########.fr       */
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
