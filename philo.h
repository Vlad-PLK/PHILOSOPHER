/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpolojie <vpolojie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 11:06:49 by vpolojie          #+#    #+#             */
/*   Updated: 2023/01/09 15:36:23 by vpolojie         ###   ########.fr       */
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
	int				meals;
	int				index;
	long long int	starting_time;
	long long int	real_time;
	int				test;
	struct s_philo	*main_phi;
}t_philo_perso;

typedef struct s_philo
{
	pthread_mutex_t		mutex_tab[201];
	pthread_mutex_t		dead_mutex;
	t_philo_perso		tab[201];
	t_phil_args			data;
	int					all_meals;
	int					is_dead;
	int					index;
	int					*forks_tab;
}t_philo;

t_phil_args		ft_init_args(int argc, char **argv);
t_philo			ft_philo_struct(int argc, char **argv);
int				ft_usleep(unsigned int time, t_philo_perso *philo);
int				ft_atoi(const char *str);
void			ft_rotate_a(t_stack *a);
long long int	ft_current_time(void);
int				try_eat(t_philo_perso *philo);

#endif