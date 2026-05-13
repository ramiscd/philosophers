/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdamasce <rdamasce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 22:37:23 by rdamasce          #+#    #+#             */
/*   Updated: 2026/05/12 22:52:21 by rdamasce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/philo.h"

static void	lock_forks(t_philo *philo, pthread_mutex_t *f, pthread_mutex_t *s)
{
	pthread_mutex_lock(f);
	print_status(philo, "has taken a fork");
	if (philo->data->philo_num == 1)
	{
		ft_usleep(philo->data->time_to_die);
		pthread_mutex_unlock(f);
		return ;
	}
	pthread_mutex_lock(s);
	print_status(philo, "has taken a fork");
}

/**
 * @brief Logic for eating. Forks are locked and timestamps updated.
 * @param philo: Current philosopher thread data.
 * @role Handles mutex locking for forks and updates last_meal time.
 */
void	philo_eat(t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	first = philo->left_fork;
	second = philo->right_fork;
	if (philo->left_fork > philo->right_fork)
	{
		first = philo->right_fork;
		second = philo->left_fork;
	}
	lock_forks(philo, first, second);
	if (philo->data->philo_num == 1)
		return ;
	print_status(philo, "is eating");
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_lock);
	ft_usleep(philo->data->time_to_eat);
	pthread_mutex_unlock(second);
	pthread_mutex_unlock(first);
}

int	check_death(t_data *data)
{
	int	res;

	pthread_mutex_lock(&data->dead_lock);
	res = data->dead_flag;
	pthread_mutex_unlock(&data->dead_lock);
	return (res);
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
		ft_usleep(1);
	while (!check_death(philo->data))
	{
		philo_eat(philo);
		if (check_death(philo->data))
			break ;
		print_status(philo, "is sleeping");
		ft_usleep(philo->data->time_to_sleep);
		print_status(philo, "is thinking");
	}
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
	pthread_t	monitor_thread;
	int			i;

	data->start_time = get_time();
	if (pthread_create(&monitor_thread, NULL, &monitor_routine, data))
		return (1);
	i = -1;
	while (++i < data->philo_num)
	{
		if (pthread_create(&data->philos[i].thread, NULL,
				&routine, &data->philos[i]))
			return (1);
	}
	pthread_join(monitor_thread, NULL);
	i = -1;
	while (++i < data->philo_num)
		pthread_join(data->philos[i].thread, NULL);
	return (0);
}
