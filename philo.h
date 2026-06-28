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
    int             philo_count;
    int             time_to_die;
    int             time_to_eat;
    int             time_to_sleep;
    int             max_meals;
    long long       start_time;
    int             is_dead;
    
    pthread_mutex_t *forks;         // Masadaki tüm çatalların (mutex) dizisi
    pthread_mutex_t print_mutex;    // Ekrana yazdırma kilidi
    pthread_mutex_t dead_mutex;     // Ölüm durumunu koruma kilidi
} t_data;

/* Filozof Veri Yapısı */
typedef struct s_philo
{
    int             id;             // Filozofun numarası (1, 2, 3...)
    int             eat_count;
    long long       last_meal_time;
    
    pthread_t       thread;
    
    pthread_mutex_t *first_fork;    // Önce alınacak çatal (Asimetrik mantık için)
    pthread_mutex_t *second_fork;   // Sonra alınacak çatal (Asimetrik mantık için)
    
    t_data          *data;
} t_philo;

#endif