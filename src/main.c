#include "../src/philo.h"

/**
 * @brief Converte string para inteiro positivo.
 * @param str: "200"
 * @return 200 (ou -1 se inválido)
 * @details Valida se a entrada contém apenas dígitos e cabe em um int positivo.
 */
int	ft_atoi_positive(char *str)
{
	long	res;
	int		i;

	res = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		res = res * 10 + (str[i] - '0');
		if (res > 2147483647)
			return (-1);
		i++;
	}
	return ((int)res);
}

/**
 * @brief Valida e armazena os argumentos na estrutura de dados.
 * @param data: endereço da struct t_data
 * @param argv: vetor de argumentos do main
 * @return 0 se sucesso, 1 se erro
 * @details Preenche a mesa com os tempos e contagens iniciais.
 */
int	init_args(t_data *data, int argc, char **argv)
{
	data->philo_num = ft_atoi_positive(argv[1]);
	data->time_to_die = ft_atoi_positive(argv[2]);
	data->time_to_eat = ft_atoi_positive(argv[3]);
	data->time_to_sleep = ft_atoi_positive(argv[4]);
	data->meals_limit = -1;
	if (argc == 6)
		data->meals_limit = ft_atoi_positive(argv[5]);
	if (data->philo_num < 1 || data->time_to_die < 0 || data->time_to_eat < 0
		|| data->time_to_sleep < 0 || (argc == 6 && data->meals_limit < 0))
		return (1);
	return (0);
}

void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_num)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->write_lock);
	free(data->forks);
	if (data->philos)
		free(data->philos);
}

int	main(int argc, char **argv)
{
	t_data	data;
	data.forks = NULL;
	data.philos = NULL; // Safety

	if (argc < 5 || argc > 6)
		return (printf("Error: Wrong arguments\n"), 1);
	if (init_args(&data, argc, argv))
		return (printf("Error: Invalid values\n"), 1);
	if (init_mutexes(&data))
		return (printf("Error: Mutex init failed\n"), 1);
	if (init_philos(&data))
		return (printf("Error: Philo init failed\n"), cleanup(&data), 1);
	if (start_simulation(&data))
		return (printf("Error: Thread creation failed\n"), cleanup(&data), 1);

	data.start_time = get_time();

	cleanup(&data);
	return (0);
}
