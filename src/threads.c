#include "../src/philo.h"

/**
 * @brief Logic for eating. Forks are locked and timestamps updated.
 * @param philo: Current philosopher thread data.
 * @role Handles mutex locking for forks and updates last_meal time.
 */
void	philo_eat(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(philo->right_fork);
	print_status(philo, "has taken a fork");
	
	print_status(philo, "is eating");
	philo->last_meal = get_time(); // Critical for death check later
	ft_usleep(philo->data->time_to_eat);
	philo->meals_eaten++;
	
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

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
	if (philo->id % 2 == 0)
		ft_usleep(10); // Simple trick to prevent deadlock
	
	// Por enquanto, vamos fazer apenas 1 ciclo para testar
	philo_eat(philo);
	print_status(philo, "is sleeping");
	ft_usleep(philo->data->time_to_sleep);
	print_status(philo, "is thinking");
	
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
