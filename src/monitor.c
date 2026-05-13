/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdamasce <rdamasce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 22:37:06 by rdamasce          #+#    #+#             */
/*   Updated: 2026/05/12 22:43:42 by rdamasce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/philo.h"

/**
 * @brief Checks if all philosophers have reached their meal limit.
 * @param data: Main simulation data.
 * @return 1 if all are full, 0 otherwise.
 */
static int	all_philos_full(t_data *data)
{
	int	i;
	int	full_count;

	if (data->meals_limit == -1)
		return (0);
	full_count = 0;
	i = -1;
	while (++i < data->philo_num)
	{
		pthread_mutex_lock(&data->philos[i].meal_lock);
		if (data->philos[i].meals_eaten >= data->meals_limit)
			full_count++;
		pthread_mutex_unlock(&data->philos[i].meal_lock);
	}
	if (full_count == data->philo_num)
	{
		pthread_mutex_lock(&data->dead_lock);
		data->dead_flag = 1;
		pthread_mutex_unlock(&data->dead_lock);
		return (1);
	}
	return (0);
}

/**
 * @brief Checks if a philosopher has died of starvation.
 * @return 1 if someone died, 0 otherwise.
 * @role Periodic check to enforce time_to_die.
 */
static int	is_philo_dead(t_philo *philo)
{
	long	time_since_last_meal;

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
void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (1)
	{
		i = -1;
		while (++i < data->philo_num)
		{
			if (is_philo_dead(&data->philos[i]))
			{
				print_status(&data->philos[i], "died");
				pthread_mutex_lock(&data->dead_lock);
				data->dead_flag = 1;
				pthread_mutex_unlock(&data->dead_lock);
				return (NULL);
			}
		}
		if (all_philos_full(data))
			return (NULL);
		usleep(1000);
	}
}
