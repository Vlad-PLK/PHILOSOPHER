/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpolojie <vpolojie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 11:06:49 by vpolojie          #+#    #+#             */
/*   Updated: 2022/12/15 13:21:44 by vpolojie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_stack
{
	int	size_max;
	int	top_index;
	int	*tableau;
}t_stack;

typedef struct s_phil_args
{
	int	nbr_phils;
	int	tm_die;
	int	tm_eat;
	int	tm_sleep;
	int	nbr_meals;
}t_phil_args;

typedef struct s_philo
{
	pthread_t	*phil_tab;
	t_phil_args	*data;
	t_stack		*stack;
	int			*forks_tab;
	int			*index;
}t_philo;

int	ft_atoi(const char *str);

#endif