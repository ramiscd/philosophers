#include "../src/philo.h"

/**
 * @brief Initializes all mutexes for the simulation.
 * @param data: Pointer to the main data structure.
 * @return 0 on success, 1 on error.
 * @example init_mutexes(data) -> returns 0 if all 5 forks + write_lock are ready.
 * @role Allocates memory for forks and initializes each as a mutex.
 */
int	init_mutexes(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->philo_num);
	if (!data->forks)
		return (1);
	i = 0;
	while (i < data->philo_num)
	{
		if (pthread_mutex_init(&data->forks[i], NULL))
			return (1);
		i++;
	}
	if (pthread_mutex_init(&data->write_lock, NULL))
		return (1);
	return (0);
}

/**
 * @brief Initializes each philosopher and assigns their forks.
 * @param data: Pointer to the main data structure.
 * @return 0 on success, 1 on memory error.
 * @example init_philos(data) -> Philo 1 gets fork 0 and 1.
 * @role Connects each philo to the shared data and their specific mutexes.
 */
int	init_philos(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->philo_num);
	if (!data->philos)
		return (1);
	i = 0;
	while (i < data->philo_num)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal = 0; // Will be updated at start
		data->philos[i].data = data;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->philo_num];
		i++;
	}
	return (0);
}