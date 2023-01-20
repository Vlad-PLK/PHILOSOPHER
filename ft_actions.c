/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_actions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpolojie <vpolojie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 16:46:55 by vpolojie          #+#    #+#             */
/*   Updated: 2023/01/20 12:46:55 by vpolojie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_sleep(t_philo_perso *philo)
{
	if (check_meals(philo) == -1)
		ft_printf(philo, philo->index, "is sleeping");
	if (ft_usleep(philo->main_phi->data.tm_sleep, philo) == -1)
		return (-1);
	else
		return (1);
}

void	think(t_philo_perso *philo)
{
	if (philo->is_thinking == 0)
	{
		if (check_meals(philo) == -1)
			ft_printf(philo, philo->index, "is thinking");
		philo->is_thinking = 1;
	}
	return ;
}

int	ft_second_fork(t_philo_perso *philo, int i, int neighbour)
{
	pthread_mutex_lock(&philo->main_phi->mutex_tab[neighbour]);
	if (philo->main_phi->forks_tab[neighbour] == 1
		&& check_meals(philo) == -1)
	{
		ft_printf(philo, i, "has taken a fork");
		ft_printf(philo, i, "is eating");
	}
	philo->main_phi->forks_tab[neighbour] = 0;
	pthread_mutex_lock(&philo->main_phi->meal_mutex);
	philo->meals++;
	pthread_mutex_unlock(&philo->main_phi->meal_mutex);
	if (ft_usleep(philo->main_phi->data.tm_eat, philo) == -1
		|| check_meals(philo) == 1)
	{
		pthread_mutex_unlock(&philo->main_phi->mutex_tab[i]);
		pthread_mutex_unlock(&philo->main_phi->mutex_tab[neighbour]);
		return (-1);
	}
	philo->starting_time = ft_current_time();
	philo->main_phi->forks_tab[i] = 1;
	pthread_mutex_unlock(&philo->main_phi->mutex_tab[i]);
	philo->main_phi->forks_tab[neighbour] = 1;
	pthread_mutex_unlock(&philo->main_phi->mutex_tab[neighbour]);
	return (1);
}

int	eat(t_philo_perso *philo)
{
	int	i;
	int	neighbour;

	i = philo->index;
	neighbour = i -1;
	pthread_mutex_lock(&philo->main_phi->mutex_tab[i]);
	if (philo->main_phi->forks_tab[i] == 1
		&& check_meals(philo) == -1)
		ft_printf(philo, i, "has taken a fork");
	philo->main_phi->forks_tab[i] = 0;
	if (i == 0)
		neighbour = philo->main_phi->data.nbr_phils -1;
	if (ft_second_fork(philo, i, neighbour) == -1)
		return (-1);
	if (check_meals(philo) == 1)
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
