/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycinarog <ycinarog@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 01:05:13 by ycinarog          #+#    #+#             */
/*   Updated: 2026/06/30 10:50:48 by ycinarog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	clean_up(t_data *data, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->dead_mutex);
	free(data->forks);
	free(philos);
}

static int	check_args(int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

static int	error_msg(char *str)
{
	printf("%s\n", str);
	return (1);
}

int	main(int argc, char **argv)
{
	t_data	data;
	t_philo	*philos;

	if (argc != 5 && argc != 6)
		return (error_msg("Error: Wrong number of arguments"));
	if (!check_args(argc, argv))
		return (error_msg("Error: Invalid arguments"));
	if (!init_data(&data, argc, argv))
		return (error_msg("Error: Initialization failed or invalid values"));
	if (!init_mutexes(&data))
		return (error_msg("Error: Mutex init failed"));
	if (!init_philos(&data, &philos))
		return (error_msg("Error: Philo init failed"));
	start_simulation(&data, philos);
	clean_up(&data, philos);
	return (0);
}
