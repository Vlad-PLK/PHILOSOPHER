/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpolojie <vpolojie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 11:06:49 by vpolojie          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2022/12/15 12:10:32 by vpolojie         ###   ########.fr       */
=======
/*   Updated: 2022/12/08 14:05:05 by vpolojie         ###   ########.fr       */
>>>>>>> d8b7b80160b6034563438afef1a7f5dbdb690d81
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>

<<<<<<< HEAD
typedef struct s_stack
{
	int	size_max;
	int	top_index;
	int	*tableau;
}t_stack;

=======
>>>>>>> d8b7b80160b6034563438afef1a7f5dbdb690d81
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
<<<<<<< HEAD
	t_stack		*stack;
	int			*forks_tab;
	int			*index;
=======
	int			*forks_tab;
>>>>>>> d8b7b80160b6034563438afef1a7f5dbdb690d81
}t_philo;

int	ft_atoi(const char *str);

#endif