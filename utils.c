/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycinarog <ycinarog@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 01:05:35 by ycinarog          #+#    #+#             */
/*   Updated: 2026/06/30 19:21:00 by ycinarog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		return (-1);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	ft_usleep(int time_in_ms)
{
	long long	start_time;

	start_time = get_current_time();
	while ((get_current_time() - start_time) < time_in_ms)
		usleep(500);
}

int	check_if_dead(t_data *data)
{
	int	status;

	pthread_mutex_lock(&data->dead_mutex);
	status = data->is_dead;
	pthread_mutex_unlock(&data->dead_mutex);
	return (status);
}

long long	ft_atoi(const char *nptr)
{
	int		i;
	long	result;

	i = 0;
	result = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i] == '+')
		i++;
	else if (nptr[i] == '-') 
		return (-1);
	if (nptr[i] < '0' || nptr[i] > '9')
		return (-1);
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = result * 10 + (nptr[i] - '0');
		if (result > 2147483647)
			return (-1);
		i++;
	}
	if (nptr[i] != '\0')
		return (-1);
	return (result);
}
