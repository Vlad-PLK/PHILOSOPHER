/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpolojie <vpolojie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 13:08:37 by vpolojie          #+#    #+#             */
/*   Updated: 2023/01/03 19:07:35 by vpolojie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_sleep(t_philo_perso *philo)
{
	if (philo->main_phi->is_dead != 1)
	{
		printf("%lld %d is sleeping\n", ft_current_time() - philo->real_time, philo->index);
		if (ft_usleep(philo->main_phi->data.tm_sleep, philo) == -1)
			return (-1);
		else
			return (1);
	}
	return (-1);
}

void	think(t_philo_perso *philo)
{
	if (philo->is_thinking == 0)
	{
		printf("%lld %d is thinking\n", ft_current_time() - philo->real_time, philo->index);
		philo->is_thinking = 1;
	}
	return ;
}

int	eat(t_philo_perso *philo)
{
	int	i;

	i = philo->index;
	pthread_mutex_lock(&philo->main_phi->mutex_tab[i]);
	philo->main_phi->forks_tab[i] = 0;
	printf("%lld %d has taken a fork\n", ft_current_time() - philo->real_time, i);
	if (i == 0)
	{
		pthread_mutex_lock(&philo->main_phi->mutex_tab[philo->main_phi->data.nbr_phils -1]);
		philo->main_phi->forks_tab[philo->main_phi->data.nbr_phils -1] = 0;
		printf("%lld %d has taken a fork\n", ft_current_time() - philo->real_time, i);
	}
	else
	{
		pthread_mutex_lock(&philo->main_phi->mutex_tab[i -1]);
		philo->main_phi->forks_tab[i -1] = 0;
		printf("%lld %d has taken a fork\n", ft_current_time() - philo->real_time, i);
	}
	printf("%lld %d is eating\n", ft_current_time() - philo->real_time, i);
	if (ft_usleep(philo->main_phi->data.tm_eat, philo) == -1)
		return (-1);
	philo->starting_time = ft_current_time();
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
	philo->meals++;
	philo->is_thinking = 0;
	if (philo->meals != philo->main_phi->data.nbr_meals)
	{
		if (ft_sleep(philo) == -1)
			return (-1);
	}
	return (1);
}

int	try_eat(t_philo_perso *philo)
{
	int	i;

	i = philo->index;
	if (philo->main_phi->is_dead != 1)
	{
		think(philo);
		if (eat(philo) == -1)
			return (-1);
		else
			return (1);
	}
	return (-1);
}

int	check_meals(t_philo_perso *philo)
{
	int	i;

	i = 0;
	while (i != (philo->main_phi->data.nbr_phils -1))
	{
		if (philo->main_phi->tab[i].meals == philo->main_phi->data.nbr_meals)
			i++;
		else
			return (-1);
	}
	return (1);
}

void	*start_philo(void *arg)
{
	t_philo_perso	*philo;
	struct timeval	current_time;

	philo = (t_philo_perso *)arg;
	while (philo->main_phi->index != (philo->main_phi->data.nbr_phils -1))
		gettimeofday(&current_time, NULL);
	philo->real_time = ft_current_time();
	philo->starting_time = ft_current_time();
	philo->is_thinking = 0;
	philo->meals = 0;
	if (philo->index % 2 != 0)
		ft_usleep(5, philo);
	while ((ft_current_time() - philo->starting_time
			<= philo->main_phi->data.tm_die)
		|| philo->main_phi->data.nbr_meals == 0)
	{
		gettimeofday(&current_time, NULL);
		if (philo->main_phi->data.nbr_phils == 1)
			think(philo);
		else
		{
			if (check_nb_meals(philo) == 1)
				return (0);
			if (try_eat(philo) == -1)
			{
				if (philo->main_phi->is_dead == 1)
					return (0);
				else
					break ;
			}
		}
	}
	pthread_mutex_lock(&philo->main_phi->dead_mutex);
	if (philo->main_phi->is_dead == 0)
		printf("%lld %d died\n", ft_current_time() - philo->real_time,
			philo->index);
	philo->main_phi->is_dead = 1;
	pthread_mutex_unlock(&philo->main_phi->dead_mutex);
	return (0);
}

int	philo(int argc, char **argv)
{
	t_philo			philo;
	int				i;

	philo = ft_philo_struct(argc, argv);
	i = 0;
	philo.is_dead = 0;
	pthread_mutex_init(&philo.dead_mutex, NULL);
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
		if (pthread_create(&philo.tab[i].thread_index, NULL,
				&start_philo, &philo.tab[i]) != 0)
		{
			perror("Failed to create thread\n");
			return (1);
		}
		i++;
	}
	i = 0;
	while (i != philo.data.nbr_phils)
	{
		if (pthread_join(philo.tab[i].thread_index, NULL) != 0)
			return (2);
		i++;
	}
	i = 0;
	while (i != philo.data.nbr_phils)
	{
		pthread_mutex_destroy(&philo.mutex_tab[i]);
		i++;
	}
	pthread_mutex_destroy(&philo.dead_mutex);
	return (0);
}

int	main(int argc, char **argv)
{
	if (argc == 5 || argc == 6)
	{
		if (ft_atoi(argv[1]) >= 1)
		{
			if (ft_atoi(argv[1]) <= 200)
				philo(argc, argv);
			else
				printf("200 philosophers max\n");
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