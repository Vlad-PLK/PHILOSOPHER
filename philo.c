/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpolojie <vpolojie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 13:08:37 by vpolojie          #+#    #+#             */
/*   Updated: 2023/01/16 18:36:03 by vpolojie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_sleep(t_philo_perso *philo)
{
	if (philo->main_phi->is_dead != 1)
	{
		if (check_meals(philo) == -1)
			printf("%lld %d is sleeping\n",
				ft_current_time() - philo->real_time, philo->index);
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
		if (check_meals(philo) == -1)
			printf("%lld %d is thinking\n",
				ft_current_time() - philo->real_time, philo->index);
		philo->is_thinking = 1;
	}
	return ;
}

int	check_meals(t_philo_perso *philo)
{
	int	i;

	i = 0;
	if (philo->main_phi->data.nbr_meals == 0)
		return (-1);
	pthread_mutex_lock(&philo->main_phi->meal_mutex);
	while (i <= (philo->main_phi->data.nbr_phils -1))
	{
		if (philo->main_phi->tab[i].meals < philo->main_phi->data.nbr_meals)
		{
			pthread_mutex_unlock(&philo->main_phi->meal_mutex);
			return (-1);
		}
		i++;
	}
	philo->main_phi->all_meals = 1;
	pthread_mutex_unlock(&philo->main_phi->meal_mutex);
	return (1);
}

int	ft_first_philo(t_philo_perso *philo, int i)
{
	int	first;

	first = philo->main_phi->data.nbr_phils -1;
	pthread_mutex_lock(&philo->main_phi->mutex_tab[first]);
	if (philo->main_phi->forks_tab[first] == 1
		&& check_meals(philo) == -1)
	{
		printf("%lld %d has taken a fork\n",
			ft_current_time() - philo->real_time, i);
		printf("%lld %d is eating\n", ft_current_time() - philo->real_time, i);
	}
	philo->main_phi->forks_tab[first] = 0;
	philo->meals++;
	if (philo->main_phi->data.nbr_meals > 0)
	{
		if (check_meals(philo) == 1)
		{
			pthread_mutex_unlock(&philo->main_phi->mutex_tab[i]);
			pthread_mutex_unlock(&philo->main_phi->mutex_tab[first]);
			return (-1);
		}
	}
	printf("Philo n'%d, meal n'%d\n", philo->index, philo->meals);
	if (ft_usleep(philo->main_phi->data.tm_eat, philo) == -1)
	{
		pthread_mutex_unlock(&philo->main_phi->mutex_tab[i]);
		pthread_mutex_unlock(&philo->main_phi->mutex_tab[first]);
		return (-1);
	}
	philo->starting_time = ft_current_time();
	philo->main_phi->forks_tab[i] = 1;
	pthread_mutex_unlock(&philo->main_phi->mutex_tab[i]);
	philo->main_phi->forks_tab[philo->main_phi->data.nbr_phils -1] = 1;
	pthread_mutex_unlock(&philo->main_phi->mutex_tab[first]);
	return (1);
}

int	ft_others_philo(t_philo_perso *philo, int i)
{
	pthread_mutex_lock(&philo->main_phi->mutex_tab[i -1]);
	if (philo->main_phi->forks_tab[i -1] == 1
		&& check_meals(philo) == -1)
	{
		printf("%lld %d has taken a fork\n",
			ft_current_time() - philo->real_time, i);
		printf("%lld %d is eating\n", ft_current_time() - philo->real_time, i);
	}
	philo->main_phi->forks_tab[i -1] = 0;
	philo->meals++;
	if (philo->main_phi->data.nbr_meals > 0)
	{
		if (check_meals(philo) == 1)
		{
			pthread_mutex_unlock(&philo->main_phi->mutex_tab[i]);
			pthread_mutex_unlock(&philo->main_phi->mutex_tab[i -1]);
			return (-1);
		}
	}
	printf("Philo n'%d, meal n'%d\n", philo->index, philo->meals);
	if (ft_usleep(philo->main_phi->data.tm_eat, philo) == -1)
	{
		pthread_mutex_unlock(&philo->main_phi->mutex_tab[i]);
		pthread_mutex_unlock(&philo->main_phi->mutex_tab[i -1]);
		return (-1);
	}
	philo->starting_time = ft_current_time();
	philo->main_phi->forks_tab[i] = 1;
	pthread_mutex_unlock(&philo->main_phi->mutex_tab[i]);
	philo->main_phi->forks_tab[i -1] = 1;
	pthread_mutex_unlock(&philo->main_phi->mutex_tab[i -1]);
	return (1);
}

int	eat(t_philo_perso *philo)
{
	int	i;

	i = philo->index;
	pthread_mutex_lock(&philo->main_phi->mutex_tab[i]);
	if (philo->main_phi->forks_tab[i] == 1
		&& check_meals(philo) == -1)
		printf("%lld %d has taken a fork\n",
			ft_current_time() - philo->real_time, i);
	philo->main_phi->forks_tab[i] = 0;
	if (i == 0)
	{
		if (ft_first_philo(philo, i) == -1)
			return (-1);
	}
	else
	{
		if (ft_others_philo(philo, i) == -1)
			return (-1);
	}
	if (philo->main_phi->data.nbr_meals > 0 && check_meals(philo) == 1)
		return (-1);
	philo->is_thinking = 0;
	ft_sleep(philo);
	return (1);
}

int	try_eat(t_philo_perso *philo)
{
	think(philo);
	if (eat(philo) == -1)
		return (-1);
	else
		return (1);
	return (-1);
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
	while (ft_current_time() - philo->starting_time
		<= philo->main_phi->data.tm_die || check_meals(philo) == -1)
	{
		gettimeofday(&current_time, NULL);
		if (philo->main_phi->data.nbr_phils == 1)
			think(philo);
		else
		{
			if (check_meals(philo) == -1)
				if (try_eat(philo) == -1)
					break ;
		}
	}
	pthread_mutex_lock(&philo->main_phi->dead_mutex);
	if (philo->main_phi->is_dead == 0 && philo->main_phi->all_meals == 0)
		printf("%lld %d died\n", ft_current_time() - philo->real_time,
			philo->index);
	philo->main_phi->is_dead = 1;
	pthread_mutex_unlock(&philo->main_phi->dead_mutex);
	return (0);
}

void	ft_initialize_mutex(t_philo *philo)
{
	int	i;

	i = 0;
	pthread_mutex_init(&philo->dead_mutex, NULL);
	pthread_mutex_init(&philo->meal_mutex, NULL);
	while (i != philo->data.nbr_phils)
	{
		pthread_mutex_init(&philo->mutex_tab[i], NULL);
		i++;
	}
}

void	ft_destroy_mutex(t_philo *philo)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&philo->dead_mutex);
	pthread_mutex_destroy(&philo->meal_mutex);
	while (i != philo->data.nbr_phils)
	{
		pthread_mutex_destroy(&philo->mutex_tab[i]);
		i++;
	}
}

int	philo(int argc, char **argv)
{
	t_philo			philo;
	int				i;

	philo = ft_philo_struct(argc, argv);
	ft_initialize_mutex(&philo);
	i = 0;
	philo.is_dead = 0;
	philo.all_meals = 0;
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
	ft_destroy_mutex(&philo);
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
