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