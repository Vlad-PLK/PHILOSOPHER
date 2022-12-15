/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpolojie <vpolojie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 11:10:54 by vpolojie          #+#    #+#             */
/*   Updated: 2022/12/08 11:11:22 by vpolojie         ###   ########.fr       */
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