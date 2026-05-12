#include "../src/philo.h"

/**
 * @brief Gets the current time in milliseconds.
 * @return Current time as a long in ms.
 * @example get_time() -> 1715456258123
 * @role Used to calculate timestamps for logs and death monitoring.
 */
long	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (0);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/**
 * @brief Custom sleep function for better precision.
 * @param time_in_ms: Time to sleep in milliseconds.
 * @role Replaces usleep to ensure the simulation doesn't drift too much.
 */
void	ft_usleep(long time_in_ms)
{
	long	start;

	start = get_time();
	while ((get_time() - start) < time_in_ms)
		usleep(500);
}

/**
 * @brief Prints philosopher actions with timestamps.
 * @param philo: Current philosopher.
 * @param str: Action (e.g., "has taken a fork").
 * @role Thread-safe logging using write_lock.
 */
void	print_status(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->data->write_lock);
	printf("%ld %d %s\n", get_time() - philo->data->start_time, philo->id, str);
	pthread_mutex_unlock(&philo->data->write_lock);
}