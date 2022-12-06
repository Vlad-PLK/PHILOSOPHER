/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpolojie <vpolojie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 13:08:37 by vpolojie          #+#    #+#             */
/*   Updated: 2022/12/06 15:49:36 by vpolojie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

void	*routine()
{
	printf("test from thread\n");
	sleep(3);
	printf("end of test\n");
}

int	main(int argc, char **argv)
{
	pthread_t	t1;
	pthread_create(&t1, NULL, &routine, NULL);
	pthread_join(t1, NULL);
	return (0);
}