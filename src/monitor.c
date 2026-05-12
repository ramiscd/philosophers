#include "../src/philo.h"

/**
 * @brief Checks if a philosopher has died of starvation.
 * @return 1 if someone died, 0 otherwise.
 * @role Periodic check to enforce time_to_die.
 */
static int  is_philo_dead(t_philo *philo)
{
	long    time_since_last_meal;

	pthread_mutex_lock(&philo->meal_lock);
	time_since_last_meal = get_time() - philo->last_meal;
	if (time_since_last_meal >= philo->data->time_to_die)
	{
		pthread_mutex_unlock(&philo->meal_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->meal_lock);
	return (0);
}

/**
 * @brief Monitor loop that watches all philosophers.
 * @role Stops the simulation if someone dies or all ate enough.
 */
void    *monitor_routine(void *arg)
{
	t_data  *data;
	int     i;

	data = (t_data *)arg;
	while (1)
	{
		i = 0;
		while (i < data->philo_num)
		{
			if (is_philo_dead(&data->philos[i]))
			{
				print_status(&data->philos[i], "died");
				pthread_mutex_lock(&data->dead_lock);
				data->dead_flag = 1;
				pthread_mutex_unlock(&data->dead_lock);
				return (NULL);
			}
			i++;
		}
		usleep(1000); // Evita sobrecarga da CPU
	}
}