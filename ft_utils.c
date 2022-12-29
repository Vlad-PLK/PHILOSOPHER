/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpolojie <vpolojie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 11:10:54 by vpolojie          #+#    #+#             */
/*   Updated: 2022/12/29 16:02:29 by vpolojie         ###   ########.fr       */
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

void	ft_rotate_a(t_stack *a)
{
	int	temp;
	int	v_haut;

	v_haut = (a->size_max) - (a->top_index) - 1;
	while (v_haut != 0)
	{
		temp = a->tableau[v_haut];
		a->tableau[v_haut] = a->tableau[v_haut -1];
		a->tableau[v_haut -1] = temp;
		v_haut--;
	}
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
	pile_a = create_stack(philo->data.nbr_phils);
	while (i != philo->data.nbr_phils)
	{
		pile_a->top_index--;
		pile_a->tableau[pile_a->top_index] = i;
		i++;
	}
	return (pile_a);
}

long long int	ft_current_time(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000) + current_time.tv_usec / 1000);
}