/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycinarog <ycinarog@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 00:06:01 by ycinarog          #+#    #+#             */
/*   Updated: 2026/06/30 00:06:12 by ycinarog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philo.h"

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