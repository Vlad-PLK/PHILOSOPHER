/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpolojie <vpolojie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 11:06:49 by vpolojie          #+#    #+#             */
/*   Updated: 2022/12/30 17:46:36 by vpolojie         ###   ########.fr       */
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
	pthread_t		thread_index;
	pthread_mutex_t	printf_mutex;
	int				is_thinking;
	int				eat_state;
	int				index;
	long long int	starting_time;
	int				test;
	struct s_philo	*main_phi;
}t_philo_perso;

typedef struct s_philo
{
	pthread_mutex_t		mutex_tab[201];
	t_philo_perso		tab[201];
	t_phil_args			data;
	t_stack				*stack;
	int					is_dead;
	long long int		real_time;
	int					index;
	int					*forks_tab;
}t_philo;

int				ft_atoi(const char *str);
void			ft_rotate_a(t_stack *a);
t_stack			*create_stack(int size);
t_stack			*create_stack_tab(t_stack *pile_a, t_philo *philo);
long long int	ft_current_time(void);
void			try_eat(t_philo_perso *philo, long long int rl_tm);

#endif