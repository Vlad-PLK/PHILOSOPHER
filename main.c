/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpolojie <vpolojie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 16:38:30 by vpolojie          #+#    #+#             */
/*   Updated: 2023/01/17 17:32:38 by vpolojie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	if (argc == 5 || argc == 6)
	{
		if (ft_atoi(argv[1]) >= 1)
		{
			if (ft_atoi(argv[1]) <= 200)
				philo(argc, argv);
			else
				printf("200 philosophers max\n");
		}
		else
			printf("Not enough philosophers\n");
	}
	else
	{
		printf("Not enough arguments\n");
		return (0);
	}
}
