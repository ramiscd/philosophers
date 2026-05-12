#include "../src/philo.h"

/**
 * @brief The main loop for each philosopher thread.
 * @param arg: Pointer to the specific t_philo struct.
 * @return NULL.
 * @example pthread_create(&thread, NULL, &routine, &philo);
 * @role Executes the cycle of eating, sleeping, and thinking.
 */
void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	
	// Print birth
	pthread_mutex_lock(&philo->data->write_lock);
	printf("%ld %d is thinking\n", get_time() - philo->data->start_time, philo->id);
	pthread_mutex_unlock(&philo->data->write_lock);
	
	return (NULL);
}

/**
 * @brief Spawns threads for all philosophers.
 * @param data: Main data structure.
 * @return 0 on success, 1 on thread creation error.
 * @role Initializes the simulation by creating a thread for each philo.
 */
int	start_simulation(t_data *data)
{
	int	i;

	i = 0;
	data->start_time = get_time();
	while (i < data->philo_num)
	{
		if (pthread_create(&data->philos[i].thread, NULL, &routine, &data->philos[i]))
			return (1);
		i++;
	}
	i = 0;
	while (i < data->philo_num)
	{
		if (pthread_join(data->philos[i].thread, NULL))
			return (1);
		i++;
	}
	return (0);
}
