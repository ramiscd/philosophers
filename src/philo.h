/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdamasce <rdamasce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 22:37:14 by rdamasce          #+#    #+#             */
/*   Updated: 2026/05/12 22:40:12 by rdamasce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long			last_meal;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct s_data	*data;
	pthread_mutex_t	meal_lock;
}	t_philo;

typedef struct s_data
{
	int				philo_num;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals_limit;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write_lock;
	t_philo			*philos;
	long			start_time;
	int				dead_flag;
	pthread_mutex_t	dead_lock;
}	t_data;

/* --- Prototypes --- */
int			init_mutexes(t_data *data);
int			init_args(t_data *data, int argc, char **argv);
void		cleanup(t_data *data);
int			init_philos(t_data *data);
long		get_time(void);
int			start_simulation(t_data *data);
void		*routine(void *arg);
void		philo_eat(t_philo *philo);
void		ft_usleep(long time_in_ms);
void		print_status(t_philo *philo, char *str);
int			check_death(t_data *data);
void		*monitor_routine(void *arg);

#endif
