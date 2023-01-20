/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpolojie <vpolojie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 13:08:37 by vpolojie          #+#    #+#             */
/*   Updated: 2023/01/20 11:38:27 by vpolojie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void	ft_philo_loop(t_philo_perso *philo, struct timeval current_time)
{
	while (ft_current_time() - philo->starting_time
		<= philo->main_phi->data.tm_die)
	{
		gettimeofday(&current_time, NULL);
		if (philo->main_phi->data.nbr_phils == 1)
			think(philo);
		else
			if (try_eat(philo) == -1)
				return ;
	}
}

int	ft_milkshake(t_philo_perso *philo)
{
	pthread_mutex_lock(&philo->main_phi->meal_mutex);
	if (philo->main_phi->all_meals == 1)
	{
		pthread_mutex_unlock(&philo->main_phi->meal_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->main_phi->meal_mutex);
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
	pthread_mutex_lock(&philo->main_phi->meal_mutex);
	philo->meals = 0;
	pthread_mutex_unlock(&philo->main_phi->meal_mutex);
	if (philo->index % 2 != 0)
		ft_usleep(5, philo);
	ft_philo_loop(philo, current_time);
	if (ft_milkshake(philo) == 1)
		return (0);
	pthread_mutex_lock(&philo->main_phi->dead_mutex);
	if (philo->main_phi->is_dead == 0)
		ft_printf(philo, philo->index, "died");
	philo->main_phi->is_dead = 1;
	pthread_mutex_unlock(&philo->main_phi->dead_mutex);
	return (0);
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
	ft_join_threads(&philo);
	ft_destroy_mutex(&philo);
	free(philo.forks_tab);
	return (0);
}
