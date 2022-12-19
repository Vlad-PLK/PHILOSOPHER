/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpolojie <vpolojie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 13:08:37 by vpolojie          #+#    #+#             */
/*   Updated: 2022/12/19 18:01:01 by vpolojie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_phil_args	*ft_init_args(int argc, char **argv, t_phil_args *args)
{
	args->nbr_phils = ft_atoi(argv[1]);
	args->tm_die = ft_atoi(argv[2]);
	args->tm_eat = ft_atoi(argv[3]);
	args->tm_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		args->nbr_meals = ft_atoi(argv[5]);
	else
		args->nbr_meals = 0;
	return (args);
}

void	ft_display_args(t_phil_args args)
{
	printf("Il y a %d philosophe(s)\n", args.nbr_phils);
	printf("Time to die = %d\n", args.tm_die);
	printf("Time to eat = %d\n", args.tm_eat);
	printf("Time to sleep = %d\n", args.tm_sleep);
	if (args.nbr_meals != 0)
		printf("Nbr of meals per phil = %d\n", args.nbr_meals);
}

long long int	ft_current_time(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000000) + current_time.tv_usec);
}

void	ft_usleep(unsigned int time)
{
	struct timeval	current_time;
	long long	int	real_time;

	gettimeofday(&current_time, NULL);
	real_time = (current_time.tv_sec * 1000000) + current_time.tv_usec;
	while (1)
	{
		gettimeofday(&current_time, NULL);
		if (((current_time.tv_sec * 1000000) + current_time.tv_usec) - real_time >= time * 1000)
			break ;
		usleep(100);
	}
}

int	check_order(t_philo *philo, int i)
{
	/// checker si dans la pile au dessus de i : ///
	/// il y a i +1 ou i -1, ///
	/// s'il y a i +1 ou i -1 le philo i ne peut pas manger ///

	/// NE PAS OUBLIER ///
	/// checker si c'est le philo 0 ou le philo n ///
	int	top_index;

	top_index = philo->stack->size_max - philo->stack->top_index - 1;
	while (philo->stack->tableau[top_index] != i)
	{
		if (philo->stack->tableau[top_index] == i -1
			|| philo->stack->tableau[top_index] == i +1)
			return (0);
		else
			top_index--;
	}
	return (1);
}

int	try_eat(t_philo *philo)
{
	int	i;

	i = philo->index;
	if (i == 0)
	{
		if ((philo->forks_tab[i] == 1
		&& philo->forks_tab[philo->data->nbr_phils] == 1)
		&& check_order(philo, i) == 1)
		{
			return (1);
		}
		else
			return (0);
	}
	else
	{
		if ((philo->forks_tab[i] == 1
			&& philo->forks_tab[i -1] == 1)
			&& check_order(philo, i) == 1)
		{
			return (1);
		}
		else
			return (0);
	}
}

void	eat(t_philo *philo, long long int *real_time)
{
	int				i;

	i = philo->index;
	pthread_mutex_lock(&philo->mutex_tab[i]);
	philo->forks_tab[i] = 0;
	printf("%ld %d has taken a fork\n", ft_current_time(), i);
	philo->forks_tab[i -1] = 0;
	printf("%ld %d has taken a fork\n", ft_current_time(), i);
	real_time = ft_current_time;
	printf("%ld %d is eating\n", ft_current_time(), i);
	ft_usleep(philo->data->tm_eat);
	philo->forks_tab[i] = 1;
	philo->forks_tab[i -1] = 1;
	pthread_mutex_unlock(&philo->mutex_tab[i]);
	/// changer l'ordre de la pile ///
	ft_sleep(philo);
}

void	think(t_philo *philo)
{
	printf("%ld %d is thinking\n", ft_current_time(), philo->index);
}

void	*start_philo(void *philo)
{
	t_philo			philo_data;
	struct timeval	current_time;
	long long	int	real_time;

	philo_data = *(t_philo *)philo;
	gettimeofday(&current_time, NULL);
	real_time = (current_time.tv_sec * 1000000) + current_time.tv_usec;
	while (1)
	{	
		gettimeofday(&current_time, NULL);
		if (((current_time.tv_sec * 1000000) + current_time.tv_usec)
			- real_time >= philo_data.data->tm_die * 1000)
			break ;
		else
		{
			if (try_eat(philo) == 1)
				eat(philo, &real_time);
			else
				think(philo);
		}
	}
	printf("%ld %d died\n", ft_current_time(), philo_data.index);
	free(philo_data.index);

	///enclencher time to die///
	///est ce que je peux manger///
		///oui : fonction eat///
			///fontion eat///
			///enclencher time to eat///
			///fonction sleep///
		///non : fonction think///
}

t_stack	*create_stack(int size)
{
	t_stack	*stack;

	stack = (t_stack *)malloc(sizeof(t_stack));
	stack->size_max = size;
	stack->top_index = size;
	stack->tableau = (int *)malloc(sizeof(int) * size);
	return (stack);
}

t_stack	*create_stack_tab(t_stack *pile_a, t_philo *philo)
{
	int	i;

	i = 0;
	pile_a = create_stack(philo->data->nbr_phils);
	while (i != philo->data->nbr_phils)
	{
		pile_a->top_index--;
		pile_a->tableau[pile_a->top_index] = i;
		i++;
	}
	return (pile_a);
}

void	ft_philo_struct(t_philo *philo, int argc, char **argv)
{
	int	i;

	i = 0;
	philo->phil_tab = (pthread_t *)malloc(sizeof(pthread_t) * philo->data->nbr_phils);
	philo->mutex_tab = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
		* philo->data->nbr_phils);
	philo->data = ft_init_args(argc, argv, philo->data);
	philo->stack = create_stack_tab(philo->stack, philo);
	philo->forks_tab = (int *)malloc(sizeof(int) * philo->data->nbr_phils);
	while (i != philo->data->nbr_phils)
	{
		philo->forks_tab[i] = 1;
		i++;
	}
}

int	philo(int argc, char **argv)
{
	t_philo			philo;
	int				i;

	i = 0;
	ft_philo_struct(&philo, argc, argv);
	while (i != philo.data->nbr_phils)
	{
		pthread_mutex_init(&philo.mutex_tab[i], NULL);
		i++;
	}
	while (i != philo.data->nbr_phils)
	{
		philo.index = (int *)malloc(sizeof(int));
		*philo.index = i;
		if (pthread_create(philo.phil_tab + i, NULL, &start_philo, &philo) != 0)
		{
			perror("Failed to create thread\n");
			return (1);
		}
		printf("Thread %d created\n", i);
		i++;
	}
	i = 0;
	while (i != philo.data->nbr_phils)
	{
		if (pthread_join(philo.phil_tab[i], NULL) != 0)
			return (2);
		printf("Thread %d died\n", i);
		i++;
	}
	i = 0;
	while (i != philo.data->nbr_phils)
	{
		pthread_mutex_destroy(&philo.mutex_tab[i]);
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	if (argc == 5 || argc == 6)
	{
		if (ft_atoi(argv[1]) >= 1)
			philo(argc, argv);
		else
			printf("Not enough philosophers\n");
	}
	else
	{
		printf("Not enough arguments\n");
		return (0);
	}
}