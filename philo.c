/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpolojie <vpolojie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 13:08:37 by vpolojie          #+#    #+#             */
/*   Updated: 2022/12/30 18:35:34 by vpolojie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_phil_args	ft_init_args(int argc, char **argv)
{
	t_phil_args	args;

	args.nbr_phils = ft_atoi(argv[1]);
	args.tm_die = ft_atoi(argv[2]);
	args.tm_eat = ft_atoi(argv[3]);
	args.tm_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		args.nbr_meals = ft_atoi(argv[5]);
	else
		args.nbr_meals = 0;
	return (args);
}

void	ft_usleep(unsigned int time, t_philo_perso *philo)
{
	struct timeval	current_time;
	long long	int	real_time;

	gettimeofday(&current_time, NULL);
	real_time = (current_time.tv_sec * 1000000) + current_time.tv_usec;
	philo->main_phi->is_dead = 0;
	while (1)
	{
		gettimeofday(&current_time, NULL);
		if (ft_current_time() - philo->starting_time >= philo->main_phi->data.tm_die)
		{
			philo->main_phi->is_dead = 1;
			break ;
		}
		if (((current_time.tv_sec * 1000000) + current_time.tv_usec) - real_time >= time * 1000)
			break ;
		usleep(100);
	}
}

void	ft_sleep(t_philo_perso *philo, long long int rl_tm)
{
	printf("%lld %d is sleeping\n", ft_current_time() - rl_tm, philo->index);
	ft_usleep(philo->main_phi->data.tm_sleep, philo);
}

void	think(t_philo_perso *philo, long long int st_time)
{
	if (philo->is_thinking == 0)
	{
		printf("%lld %d is thinking\n", ft_current_time() - st_time, philo->index);
		philo->is_thinking = 1;
	}
	return ;
}

void	eat(t_philo_perso *philo, long long int rl_tm)
{
	int	i;

	i = philo->index;
	pthread_mutex_lock(&philo->main_phi->mutex_tab[i]);
	philo->main_phi->forks_tab[i] = 0;
	printf("%lld %d has taken a fork\n", ft_current_time() - rl_tm, i);
	if (i == 0)
	{
		pthread_mutex_lock(&philo->main_phi->mutex_tab[philo->main_phi->data.nbr_phils -1]);
		philo->main_phi->forks_tab[philo->main_phi->data.nbr_phils -1] = 0;
		printf("%lld %d has taken a fork\n", ft_current_time() - rl_tm, i);
	}
	else
	{
		pthread_mutex_lock(&philo->main_phi->mutex_tab[i -1]);
		philo->main_phi->forks_tab[i -1] = 0;
		printf("%lld %d has taken a fork\n", ft_current_time() - rl_tm, i);
	}
	philo->starting_time = ft_current_time();
	printf("%lld %d is eating\n", ft_current_time() - rl_tm, i);
	ft_usleep(philo->main_phi->data.tm_eat, philo);
	philo->main_phi->forks_tab[i] = 1;
	pthread_mutex_unlock(&philo->main_phi->mutex_tab[i]);
	if (i == 0)
		philo->main_phi->forks_tab[philo->main_phi->data.nbr_phils -1] = 1;
	else
		philo->main_phi->forks_tab[i -1] = 1;
	if (i == 0)
		pthread_mutex_unlock(&philo->main_phi->mutex_tab[philo->main_phi->data.nbr_phils -1]);
	else
		pthread_mutex_unlock(&philo->main_phi->mutex_tab[i -1]);
	philo->is_thinking = 0;
	ft_sleep(philo, rl_tm);
}

void	try_eat(t_philo_perso *philo, long long int rl_tm)
{
	///struct timeval	current_time;
	int	i;

	i = philo->index;
	think(philo, rl_tm);
	eat(philo, rl_tm);
}

void	*start_philo(void *arg)
{
	t_philo_perso	philo;
	long long	int	real_time;
	struct timeval	current_time;

	philo = *(t_philo_perso *)arg;
	while (philo.main_phi->index != (philo.main_phi->data.nbr_phils -1))
		gettimeofday(&current_time, NULL);
	real_time = ft_current_time();
	philo.starting_time = ft_current_time();
	philo.is_thinking = 0;
	if (philo.index % 2 != 0)
		ft_usleep(20, &philo);
	while (philo.main_phi->is_dead != 1 && (ft_current_time() - philo.starting_time <= philo.main_phi->data.tm_die))
	{
		gettimeofday(&current_time, NULL);
		if (philo.main_phi->data.nbr_phils == 1)
			think(&philo, real_time);
		else
			try_eat(&philo, real_time);
	}
	printf("%lld %d DIED\n", ft_current_time() - real_time, philo.index);
	return (0);
}

t_philo	ft_philo_struct(int argc, char **argv)
{
	t_philo	philo;
	int		n;

	n = 0;
	philo.data = ft_init_args(argc, argv);
	philo.stack = create_stack_tab(philo.stack, &philo);
	philo.forks_tab = (int *)malloc(sizeof(int) * philo.data.nbr_phils);
	while (n != philo.data.nbr_phils)
	{
		philo.forks_tab[n] = 1;
		n++;
	}
	return (philo);
}

int	philo(int argc, char **argv)
{
	t_philo			philo;
	int				i;

	philo = ft_philo_struct(argc, argv);
	i = 0;
	philo.is_dead = 0;
	while (i != philo.data.nbr_phils)
	{
		pthread_mutex_init(&philo.mutex_tab[i], NULL);
		i++;
	}
	i = 0;
	while (i != philo.data.nbr_phils)
	{
		philo.tab[i].index = i;
		philo.index = i;
		philo.tab[i].main_phi = &philo;
		if (pthread_create(&philo.tab[i].thread_index, NULL, &start_philo, &philo.tab[i]) != 0)
		{
			perror("Failed to create thread\n");
			return (1);
		}
		printf("Thread %d created\n", i);
		i++;
	}
	i = 0;
	while (i != philo.data.nbr_phils)
	{
		if (pthread_join(philo.tab[i].thread_index, NULL) != 0)
			return (2);
		printf("Thread %d died\n", i);
		i++;
	}
	i = 0;
	while (i != philo.data.nbr_phils)
	{
		pthread_mutex_destroy(&philo.mutex_tab[i]);
		i++;
	}
	return (0);
}

///	i = 0;


int	main(int argc, char **argv)
{
	if (argc == 5 || argc == 6)
	{
		if (ft_atoi(argv[1]) >= 1)
			philo(argc, argv);
		else
			printf("Not enough philosophers\n");
	}
	else
	{
		printf("Not enough arguments\n");
		return (0);
	}
}