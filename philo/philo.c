/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpolojie <vpolojie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 13:08:37 by vpolojie          #+#    #+#             */
/*   Updated: 2022/12/10 10:26:15 by vpolojie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*routine(pthread_mutex_t mutex)
{
	pthread_mutex_lock(&mutex);
	printf("Thread awake\n");
	pthread_mutex_unlock(&mutex);
	return (0);
}

t_phil_args	ft_init_args(int argc, char **argv, t_phil_args *args)
{
	args->nbr_phils = ft_atoi(argv[1]);
	args->tm_die = ft_atoi(argv[2]);
	args->tm_eat = ft_atoi(argv[3]);
	args->tm_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		args->nbr_meals = ft_atoi(argv[5]);
	else
		args->nbr_meals = 0;
	return (*args);
}

void	ft_display_args(t_phil_args args)
{
	printf("Il y a %d philosophe(s)\n", args.nbr_phils);
	printf("Time to die = %d\n", args.tm_die);
	printf("Time to eat = %d\n", args.tm_eat);
	printf("Time to sleep = %d\n", args.tm_sleep);
	if (args.nbr_meals != 0)
		printf("Nbr of meals per phil = %d\n", args.nbr_meals);
}

void	ft_usleep(unsigned int time)
{
	struct timeval	current_time;
	long int		microsecond;

	gettimeofday(&current_time, NULL);
	microsecond = current_time.tv_usec;
	printf("time in microsecond right know %ld \n", microsecond);
	while (1)
	{
		gettimeofday(&current_time, NULL);
		if ((current_time.tv_usec - microsecond) >= time * 1000)
			break ;
		usleep(1000);
	}
	printf("time in microsecond after time %d \n\n\n", current_time.tv_usec);

	gettimeofday(&current_time, NULL);
	printf("time in microsecond right know %d \n", current_time.tv_usec);
	usleep(1000000);
	gettimeofday(&current_time, NULL);
	printf("time in microsecond right know %d \n", current_time.tv_usec);
}

void	*start_philo(void *arg)
{
	///enclencher time to die///
	///est ce que je peux manger///
		///oui : fonction eat///
			///fontion eat///
			///enclencher time to eat///
			///fonction sleep///
		///non : fonction think///
}

int	philo(int argc, char **argv)
{
	t_phil_args		args;
	t_philo			philo;
	pthread_mutex_t	mutex;
	int				i;

	i = 0;
	args = ft_init_args(argc, argv, &args);
	philo.phil_tab = (pthread_t *)malloc(sizeof(pthread_t) * args.nbr_phils);
	philo.forks_tab = (int *)malloc(sizeof(int) * args.nbr_phils);
	philo.data = &args;
	while (i != args.nbr_phils)
	{
		philo.forks_tab[i] = 1;
		i++;
	}
	i = 0;
	pthread_mutex_init(&mutex, NULL);
	while (i != args.nbr_phils)
	{
		if (pthread_create(philo.phil_tab + i, NULL, &start_philo, NULL) != 0)
		{
			perror("Failed to create thread\n");
			return (1);
		}
		printf("Thread %d created\n", i);
		i++;
	}
	i = 0;
	while (i != args.nbr_phils)
	{
		if (pthread_join(philo.phil_tab[i], NULL) != 0)
			return (2);
		printf("Thread %d died\n", i);
		i++;
	}
	pthread_mutex_destroy(&mutex);
	return (0);
}

int	main(int argc, char **argv)
{
	if (argc == 5 || argc == 6)
	{
		if (ft_atoi(argv[1]) >= 1)
		{
			ft_usleep(1000);
			philo(argc, argv);
		}
		else
			printf("Not enough philosophers\n");
	}
	else
	{
		printf("Not enough arguments\n");
		return (0);
	}
}