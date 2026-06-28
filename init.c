#include "philo.h"

// Filozofların kişisel verilerini ve asimetrik çatallarını atayan fonksiyon
void assign_forks(t_philo *philo, t_data *data, int i)
{
    int left_fork_index;
    int right_fork_index;

    // Filozofun ID'si 1'den başlar, ancak dizi indexleri 0'dan başlar
    philo->id = i + 1;
    
    // Sol çatal kendi indexi, sağ çatal ise bir sonrakinin indexidir
    left_fork_index = i;
    right_fork_index = (i + 1) % data->philo_count; 
    
    /* ASİMETRİK ÇATAL MANTIĞI: 
       Her zaman index (ID) numarası küçük olan çatal önce alınır. */
    if (left_fork_index < right_fork_index)
    {
        philo->first_fork = &data->forks[left_fork_index];
        philo->second_fork = &data->forks[right_fork_index];
    }
    else // Sonuncu filozof (veya ID'si büyük olan taraf)
    {
        philo->first_fork = &data->forks[right_fork_index];
        philo->second_fork = &data->forks[left_fork_index];
    }
}