#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

/* Ortak Veri Yapısı (Masa) */
typedef struct s_data
{
	int				philo_count;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				max_meals;
	long long		start_time;
	int				is_dead;

	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	dead_mutex;
}					t_data;

/* Filozof Veri Yapısı */
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

// utils.c
long long			get_current_time(void);
void				ft_usleep(int time_in_ms);
long long			time_diff(long long past, long long pres);
int					check_if_dead(t_data *data);
int					ft_atoi(const char *nptr);

// init.c
void				assign_forks(t_philo *philo, t_data *data, int i);
int					init_mutexes(t_data *data);
int					init_philos(t_data *data, t_philo **philos);
int					init_data(t_data *data, int argc, char **argv);

// routine.c
void				*philosopher_routine(void *arg);
void				monitor_routine(t_data *data, t_philo *philos);
void				print_status(t_philo *philo, char *status);
int	check_if_dead(t_data *data); // Ölüm kontrolü

// simulation.c
int					start_simulation(t_data *data, t_philo *philos);

// leak.c (veya clean_up için dosyan)
void				clean_up(t_data *data, t_philo *philos);

#endif