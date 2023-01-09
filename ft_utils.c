/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpolojie <vpolojie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 11:10:54 by vpolojie          #+#    #+#             */
/*   Updated: 2023/01/09 18:47:27 by vpolojie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int		i;
	int		str0;
	int		m;

	str0 = 0;
	i = 0;
	m = 0;
	while ((0 <= str[i]) && (str[i] <= 32))
		i++;
	if ((str[i] == 45) || (str[i] == 43))
	{
		if (str[i] == 45)
			m = 1;
		i++;
	}
	if ((str[i] == 45) || (str[i] == 43))
		return (str0);
	while (('0' <= str[i]) && (str[i] <= '9'))
	{
		str0 = str0 * 10 + (str[i] - 48);
		i++;
	}
	if (m == 1)
		str0 = str0 * -1;
	return (str0);
}

long long int	ft_current_time(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000) + current_time.tv_usec / 1000);
}

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

int	ft_usleep(unsigned int time, t_philo_perso *philo)
{
	struct timeval	current_time;
	long long int	real_time;

	gettimeofday(&current_time, NULL);
	real_time = (current_time.tv_sec * 1000000) + current_time.tv_usec;
	while (1)
	{
		gettimeofday(&current_time, NULL);
		if (ft_current_time() - philo->starting_time >= philo->main_phi->data.tm_die)
			return (-1);
		if (((current_time.tv_sec * 1000000) + current_time.tv_usec) - real_time >= time * 1000)
			return (1);
		usleep(100);
	}
}

t_philo	ft_philo_struct(int argc, char **argv)
{
	t_philo	philo;
	int		n;

	n = 0;
	philo.data = ft_init_args(argc, argv);
	philo.forks_tab = (int *)malloc(sizeof(int) * philo.data.nbr_phils);
	while (n != philo.data.nbr_phils)
	{
		philo.forks_tab[n] = 1;
		n++;
	}
	return (philo);
}