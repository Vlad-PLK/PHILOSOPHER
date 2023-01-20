/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_thread_functions.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpolojie <vpolojie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 16:37:06 by vpolojie          #+#    #+#             */
/*   Updated: 2023/01/20 11:37:19 by vpolojie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_printf(t_philo_perso *philo, int index, char *msg)
{
	pthread_mutex_lock(&philo->main_phi->print_mutex);
	printf("%lld ms %d %s\n",
		ft_current_time() - philo->real_time, index +1, msg);
	pthread_mutex_unlock(&philo->main_phi->print_mutex);
}

void	ft_join_threads(t_philo *philo)
{
	int	i;

	i = 0;
	while (i != philo->data.nbr_phils)
	{
		if (pthread_join(philo->tab[i].thread_index, NULL) != 0)
			perror("Failed to join thread\n");
		i++;
	}
}

void	ft_initialize_mutex(t_philo *philo)
{
	int	i;

	i = 0;
	pthread_mutex_init(&philo->dead_mutex, NULL);
	pthread_mutex_init(&philo->meal_mutex, NULL);
	pthread_mutex_init(&philo->print_mutex, NULL);
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
	pthread_mutex_destroy(&philo->print_mutex);
	while (i != philo->data.nbr_phils)
	{
		pthread_mutex_destroy(&philo->mutex_tab[i]);
		i++;
	}
}
