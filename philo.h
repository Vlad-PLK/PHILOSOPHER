/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpolojie <vpolojie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 11:06:49 by vpolojie          #+#    #+#             */
/*   Updated: 2022/12/26 17:42:36 by vpolojie         ###   ########.fr       */
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

typedef struct s_philo_perso
{
	int	index;
}t_philo_perso;

typedef struct s_philo
{
	pthread_t			*phil_tab;
	pthread_mutex_t		*mutex_tab;
	t_philo_perso		tab[201];
	t_phil_args			data;
	t_stack				*stack;
	int					*forks_tab;
	int					*index;
	int					real_index;
}t_philo;

int		ft_atoi(const char *str);
void	ft_rotate_a(t_stack *a);

#endif