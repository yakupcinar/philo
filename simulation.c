/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycinarog <ycinarog@student.42istanbul.com  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 01:03:32 by ycinarog          #+#    #+#             */
/*   Updated: 2026/06/29 18:26:57 by ycinarog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	start_simulation(t_data *data, t_philo *philos)
{
	int	i;

	data->start_time = get_current_time();
	i = 0;
	while (i < data->philo_count)
	{
		philos[i].last_meal_time = get_current_time();
		if (pthread_create(&philos[i].thread, NULL, &philosopher_routine,
				&philos[i]) != 0)
			return (0);
		i++;
	}
	monitor_routine(data, philos);
	i = 0;
	while (i < data->philo_count)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	return (1);
}
